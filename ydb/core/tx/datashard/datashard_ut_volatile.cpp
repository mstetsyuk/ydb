#include "datashard_ut_common.h"
#include "datashard_ut_common_kqp.h"
#include "datashard_active_transaction.h"

namespace NKikimr {

using namespace NKikimr::NDataShard;
using namespace NKikimr::NDataShard::NKqpHelpers;
using namespace NSchemeShard;
using namespace Tests;

Y_UNIT_TEST_SUITE(DataShardVolatile) {

    Y_UNIT_TEST(DistributedWrite) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        Cerr << "!!! distributed write begin" << Endl;
        ExecSQL(server, sender, R"(
            UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);
            UPSERT INTO `/Root/table-2` (key, value) VALUES (20, 20);
        )");
        Cerr << "!!! distributed write end" << Endl;

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleExec(runtime, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
                )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 2 } items { uint32_value: 2 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }, "
            "{ items { uint32_value: 20 } items { uint32_value: 20 } }");

        const auto shards1 = GetTableShards(server, sender, "/Root/table-1");
        RebootTablet(runtime, shards1.at(0), sender);

        // We should see same results after restart
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleExec(runtime, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
                )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 2 } items { uint32_value: 2 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }, "
            "{ items { uint32_value: 20 } items { uint32_value: 20 } }");
    }

    Y_UNIT_TEST(DistributedWriteBrokenLock) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        // Start transaction that reads from table-1 and table-2
        TString sessionId, txId;
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleBegin(runtime, sessionId, txId, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
            )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");

        // Break lock using a blind write to table-1
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleCommit(runtime, sessionId, txId, R"(
                UPSERT INTO `/Root/table-1` (key, value) VALUES (3, 3);
                UPSERT INTO `/Root/table-2` (key, value) VALUES (30, 30);
            )"),
            "ERROR: ABORTED");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        // Verify transaction was not committed
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleExec(runtime, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
                )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 2 } items { uint32_value: 2 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");
    }

    Y_UNIT_TEST(DistributedWriteShardRestartBeforePlan) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        TVector<THolder<IEventHandle>> capturedPlans;
        auto capturePlans = [&](TTestActorRuntimeBase&, TAutoPtr<IEventHandle>& ev) -> auto {
            switch (ev->GetTypeRewrite()) {
                case TEvTxProcessing::TEvPlanStep::EventType: {
                    Cerr << "... captured TEvPlanStep" << Endl;
                    capturedPlans.emplace_back(ev.Release());
                    return TTestActorRuntime::EEventAction::DROP;
                }
            }
            return TTestActorRuntime::EEventAction::PROCESS;
        };
        auto prevObserverFunc = runtime.SetObserverFunc(capturePlans);

        TString sessionId = CreateSessionRPC(runtime, "/Root");

        Cerr << "!!! distributed write begin" << Endl;
        auto future = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);
            UPSERT INTO `/Root/table-2` (key, value) VALUES (20, 20);
        )", sessionId, "", true /* commitTx */), "/Root");

        WaitFor(runtime, [&]{ return capturedPlans.size() >= 2; }, "both captured plans");

        runtime.SetObserverFunc(prevObserverFunc);

        // Restart the first table shard
        auto shards1 = GetTableShards(server, sender, "/Root/table-1");
        RebootTablet(runtime, shards1.at(0), sender);

        // Unblock captured plan messages
        for (auto& ev : capturedPlans) {
            runtime.Send(ev.Release(), 0, true);
        }
        capturedPlans.clear();

        UNIT_ASSERT_VALUES_EQUAL(
            FormatResult(AwaitResponse(runtime, std::move(future))),
            "ERROR: UNDETERMINED");
        Cerr << "!!! distributed write end" << Endl;

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        // Verify transaction was not committed
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleExec(runtime, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
                )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");
    }

    Y_UNIT_TEST(DistributedWriteShardRestartAfterExpectation) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        auto shard1 = GetTableShards(server, sender, "/Root/table-1").at(0);

        TVector<THolder<IEventHandle>> capturedPlans;
        TVector<THolder<IEventHandle>> capturedReadSets;
        auto captureEvents = [&](TTestActorRuntimeBase&, TAutoPtr<IEventHandle>& ev) -> auto {
            switch (ev->GetTypeRewrite()) {
                case TEvTxProcessing::TEvPlanStep::EventType: {
                    const auto* msg = ev->Get<TEvTxProcessing::TEvPlanStep>();
                    if (msg->Record.GetTabletID() == shard1) {
                        Cerr << "... captured TEvPlanStep for " << msg->Record.GetTabletID() << Endl;
                        capturedPlans.emplace_back(ev.Release());
                        return TTestActorRuntime::EEventAction::DROP;
                    }
                    break;
                }
                case TEvTxProcessing::TEvReadSet::EventType: {
                    const auto* msg = ev->Get<TEvTxProcessing::TEvReadSet>();
                    if (msg->Record.GetTabletDest() == shard1) {
                        Cerr << "... captured TEvReadSet for " << msg->Record.GetTabletDest() << Endl;
                        capturedReadSets.emplace_back(ev.Release());
                        return TTestActorRuntime::EEventAction::DROP;
                    }
                    break;
                }
            }
            return TTestActorRuntime::EEventAction::PROCESS;
        };
        auto prevObserverFunc = runtime.SetObserverFunc(captureEvents);

        TString sessionId = CreateSessionRPC(runtime, "/Root");

        Cerr << "!!! distributed write begin" << Endl;
        auto future = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);
            UPSERT INTO `/Root/table-2` (key, value) VALUES (20, 20);
        )", sessionId, "", true /* commitTx */), "/Root");

        WaitFor(runtime, [&]{ return capturedPlans.size() >= 1 && capturedReadSets.size() >= 2; }, "captured plan and readsets");

        runtime.SetObserverFunc(prevObserverFunc);

        // Restart the first table shard
        RebootTablet(runtime, shard1, sender);

        UNIT_ASSERT_VALUES_EQUAL(
            FormatResult(AwaitResponse(runtime, std::move(future))),
            "ERROR: UNDETERMINED");
        Cerr << "!!! distributed write end" << Endl;

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        // Verify transaction was not committed
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleExec(runtime, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
                )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");
    }

    Y_UNIT_TEST(DistributedWriteEarlierSnapshotNotBlocked) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        TString sessionIdSnapshot, txIdSnapshot;
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleBegin(runtime, sessionIdSnapshot, txIdSnapshot, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
            )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        TVector<THolder<IEventHandle>> capturedReadSets;
        auto captureEvents = [&](TTestActorRuntimeBase&, TAutoPtr<IEventHandle>& ev) -> auto {
            switch (ev->GetTypeRewrite()) {
                case TEvTxProcessing::TEvReadSet::EventType: {
                    const auto* msg = ev->Get<TEvTxProcessing::TEvReadSet>();
                    Cerr << "... captured TEvReadSet for " << msg->Record.GetTabletDest()
                        << " with flags " << msg->Record.GetFlags() << Endl;
                    capturedReadSets.emplace_back(ev.Release());
                    return TTestActorRuntime::EEventAction::DROP;
                }
            }
            return TTestActorRuntime::EEventAction::PROCESS;
        };
        auto prevObserverFunc = runtime.SetObserverFunc(captureEvents);

        TString sessionId = CreateSessionRPC(runtime, "/Root");

        Cerr << "!!! distributed write begin" << Endl;
        auto future = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);
            UPSERT INTO `/Root/table-2` (key, value) VALUES (20, 20);
        )", sessionId, "", true /* commitTx */), "/Root");

        WaitFor(runtime, [&]{ return capturedReadSets.size() >= 4; }, "captured readsets");

        runtime.SetObserverFunc(prevObserverFunc);
        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        // Make sure snapshot transaction cannot see uncommitted changes and doesn't block on them
        UNIT_ASSERT_VALUES_EQUAL(
            KqpSimpleContinue(runtime, sessionIdSnapshot, txIdSnapshot, R"(
                SELECT key, value FROM `/Root/table-1`
                UNION ALL
                SELECT key, value FROM `/Root/table-2`
                ORDER BY key
            )"),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");
    }

    Y_UNIT_TEST(DistributedWriteLaterSnapshotBlockedThenCommit) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        TVector<THolder<IEventHandle>> capturedReadSets;
        auto captureEvents = [&](TTestActorRuntimeBase&, TAutoPtr<IEventHandle>& ev) -> auto {
            switch (ev->GetTypeRewrite()) {
                case TEvTxProcessing::TEvReadSet::EventType: {
                    const auto* msg = ev->Get<TEvTxProcessing::TEvReadSet>();
                    Cerr << "... captured TEvReadSet for " << msg->Record.GetTabletDest()
                        << " with flags " << msg->Record.GetFlags() << Endl;
                    capturedReadSets.emplace_back(ev.Release());
                    return TTestActorRuntime::EEventAction::DROP;
                }
            }
            return TTestActorRuntime::EEventAction::PROCESS;
        };
        auto prevObserverFunc = runtime.SetObserverFunc(captureEvents);

        TString sessionId = CreateSessionRPC(runtime, "/Root");

        Cerr << "!!! distributed write begin" << Endl;
        auto future = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);
            UPSERT INTO `/Root/table-2` (key, value) VALUES (20, 20);
        )", sessionId, "", true /* commitTx */), "/Root");

        WaitFor(runtime, [&]{ return capturedReadSets.size() >= 4; }, "captured readsets");

        runtime.SetObserverFunc(prevObserverFunc);
        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        TString sessionIdSnapshot = CreateSessionRPC(runtime, "/Root");
        auto snapshotReadFuture = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            SELECT key, value FROM `/Root/table-1`
            UNION ALL
            SELECT key, value FROM `/Root/table-2`
            ORDER BY key
        )", sessionIdSnapshot, "", false /* commitTx */), "/Root");

        // Let some virtual time pass
        SimulateSleep(runtime, TDuration::Seconds(1));

        // Read should be blocked, so we don't expect a reply
        UNIT_ASSERT(!snapshotReadFuture.HasValue());

        // Unblock readsets and sleep some more
        for (auto& ev : capturedReadSets) {
            runtime.Send(ev.Release(), 0, true);
        }
        SimulateSleep(runtime, TDuration::Seconds(1));

        // We expect successful commit and read including that data
        UNIT_ASSERT(snapshotReadFuture.HasValue());
        UNIT_ASSERT(future.HasValue());
        UNIT_ASSERT_VALUES_EQUAL(
            FormatResult(future.ExtractValue()),
            "<empty>");
        UNIT_ASSERT_VALUES_EQUAL(
            FormatResult(snapshotReadFuture.ExtractValue()),
            "{ items { uint32_value: 1 } items { uint32_value: 1 } }, "
            "{ items { uint32_value: 2 } items { uint32_value: 2 } }, "
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }, "
            "{ items { uint32_value: 20 } items { uint32_value: 20 } }");
    }

    Y_UNIT_TEST(DistributedWriteLaterSnapshotBlockedThenAbort) {
        TPortManager pm;
        TServerSettings serverSettings(pm.GetPort(2134));
        serverSettings.SetDomainName("Root")
            .SetUseRealThreads(false)
            .SetDomainPlanResolution(1000);

        Tests::TServer::TPtr server = new TServer(serverSettings);
        auto &runtime = *server->GetRuntime();
        auto sender = runtime.AllocateEdgeActor();

        runtime.SetLogPriority(NKikimrServices::TX_DATASHARD, NLog::PRI_TRACE);
        runtime.SetLogPriority(NKikimrServices::TX_PROXY, NLog::PRI_DEBUG);

        InitRoot(server, sender);

        CreateShardedTable(server, sender, "/Root", "table-1", 1);
        CreateShardedTable(server, sender, "/Root", "table-2", 1);

        ExecSQL(server, sender, "UPSERT INTO `/Root/table-1` (key, value) VALUES (1, 1);");
        ExecSQL(server, sender, "UPSERT INTO `/Root/table-2` (key, value) VALUES (10, 10);");

        const auto shard1 = GetTableShards(server, sender, "/Root/table-1").at(0);

        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(true);
        runtime.SetLogPriority(NKikimrServices::TABLET_EXECUTOR, NLog::PRI_DEBUG);

        size_t observedPlans = 0;
        TVector<THolder<IEventHandle>> capturedPlans;
        auto captureEvents = [&](TTestActorRuntimeBase&, TAutoPtr<IEventHandle>& ev) -> auto {
            switch (ev->GetTypeRewrite()) {
                case TEvTxProcessing::TEvPlanStep::EventType: {
                    ++observedPlans;
                    const auto* msg = ev->Get<TEvTxProcessing::TEvPlanStep>();
                    if (msg->Record.GetTabletID() == shard1) {
                        Cerr << "... captured TEvPlanStep for " << msg->Record.GetTabletID() << Endl;
                        capturedPlans.emplace_back(ev.Release());
                        return TTestActorRuntime::EEventAction::DROP;
                    }
                    break;
                }
            }
            return TTestActorRuntime::EEventAction::PROCESS;
        };
        auto prevObserverFunc = runtime.SetObserverFunc(captureEvents);

        TString sessionId = CreateSessionRPC(runtime, "/Root");

        Cerr << "!!! distributed write begin" << Endl;
        auto future = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            UPSERT INTO `/Root/table-1` (key, value) VALUES (2, 2);
            UPSERT INTO `/Root/table-2` (key, value) VALUES (20, 20);
        )", sessionId, "", true /* commitTx */), "/Root");

        WaitFor(runtime, [&]{ return observedPlans >= 2; }, "observed both plans");
        UNIT_ASSERT_VALUES_EQUAL(capturedPlans.size(), 1u);

        runtime.SetObserverFunc(prevObserverFunc);
        runtime.GetAppData(0).FeatureFlags.SetEnableDataShardVolatileTransactions(false);

        // Start reading from table-2
        TString sessionIdSnapshot = CreateSessionRPC(runtime, "/Root");
        auto snapshotReadFuture = SendRequest(runtime, MakeSimpleRequestRPC(R"(
            SELECT key, value FROM `/Root/table-2`
            ORDER BY key
        )", sessionIdSnapshot, "", false /* commitTx */), "/Root");

        // Let some virtual time pass
        SimulateSleep(runtime, TDuration::Seconds(1));

        // Read should be blocked, so we don't expect a reply
        UNIT_ASSERT(!snapshotReadFuture.HasValue());
        UNIT_ASSERT(!future.HasValue());

        // Reboot table-1 tablet and sleep a little, this will abort the write
        RebootTablet(runtime, shard1, sender);
        SimulateSleep(runtime, TDuration::Seconds(1));

        // We expect aborted commit and read without that data
        UNIT_ASSERT(snapshotReadFuture.HasValue());
        UNIT_ASSERT(future.HasValue());
        UNIT_ASSERT_VALUES_EQUAL(
            FormatResult(future.ExtractValue()),
            "ERROR: UNDETERMINED");
        UNIT_ASSERT_VALUES_EQUAL(
            FormatResult(snapshotReadFuture.ExtractValue()),
            "{ items { uint32_value: 10 } items { uint32_value: 10 } }");
    }

} // Y_UNIT_TEST_SUITE(DataShardVolatile)

} // namespace NKikimr
