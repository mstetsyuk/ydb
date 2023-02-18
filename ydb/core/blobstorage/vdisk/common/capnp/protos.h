#pragma once

#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <string>
#include <vector>
#include <optional>

#include "tevvget.capnp.h"

namespace NKikimrCapnProto {
    using namespace NKikimrCapnProto_;

    struct TMessageId {
        struct Reader : private NKikimrCapnProto_::TMessageId::Reader {
            Reader(NKikimrCapnProto_::TMessageId::Reader r) : NKikimrCapnProto_::TMessageId::Reader(r) {}
            Reader() = default;
            uint64_t GetSequenceId() const { return getSequenceId(); }
            uint64_t GetMsgId() const { return getMsgId(); }
            bool HasSequenceId() const { return getSequenceId() != 0; }
            bool HasMsgId() const { return getMsgId() != 0; }
            const NKikimrCapnProto_::TMessageId::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TMessageId::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TMessageId::Builder b) : NKikimrCapnProto_::TMessageId::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetSequenceId(const uint64_t& value) { return setSequenceId(value); }
            void SetMsgId(const uint64_t& value) { return setMsgId(value); }
            const NKikimrCapnProto_::TMessageId::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TTimestamps {
        struct Reader : private NKikimrCapnProto_::TTimestamps::Reader {
            Reader(NKikimrCapnProto_::TTimestamps::Reader r) : NKikimrCapnProto_::TTimestamps::Reader(r) {}
            Reader() = default;
            uint64_t GetSentByDSProxyUs() const { return getSentByDSProxyUs(); }
            uint64_t GetReceivedByVDiskUs() const { return getReceivedByVDiskUs(); }
            uint64_t GetSentByVDiskUs() const { return getSentByVDiskUs(); }
            uint64_t GetReceivedByDSProxyUs() const { return getReceivedByDSProxyUs(); }
            bool HasSentByDSProxyUs() const { return getSentByDSProxyUs() != 0; }
            bool HasReceivedByVDiskUs() const { return getReceivedByVDiskUs() != 0; }
            bool HasSentByVDiskUs() const { return getSentByVDiskUs() != 0; }
            bool HasReceivedByDSProxyUs() const { return getReceivedByDSProxyUs() != 0; }
            const NKikimrCapnProto_::TTimestamps::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TTimestamps::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TTimestamps::Builder b) : NKikimrCapnProto_::TTimestamps::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetSentByDSProxyUs(const uint64_t& value) { return setSentByDSProxyUs(value); }
            void SetReceivedByVDiskUs(const uint64_t& value) { return setReceivedByVDiskUs(value); }
            void SetSentByVDiskUs(const uint64_t& value) { return setSentByVDiskUs(value); }
            void SetReceivedByDSProxyUs(const uint64_t& value) { return setReceivedByDSProxyUs(value); }
            const NKikimrCapnProto_::TTimestamps::Builder& GetCapnpBase() const { return *this; }
        };
    };

    enum class EGetHandleClass {
        AsyncRead,
        FastRead,
        Discover,
        LowRead,
    };

    struct TActorId {
        struct Reader : private NKikimrCapnProto_::TActorId::Reader {
            Reader(NKikimrCapnProto_::TActorId::Reader r) : NKikimrCapnProto_::TActorId::Reader(r) {}
            Reader() = default;
            uint64_t GetRawX1() const { return getRawX1(); }
            uint64_t GetRawX2() const { return getRawX2(); }
            bool HasRawX1() const { return getRawX1() != 0; }
            bool HasRawX2() const { return getRawX2() != 0; }
            const NKikimrCapnProto_::TActorId::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TActorId::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TActorId::Builder b) : NKikimrCapnProto_::TActorId::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetRawX1(const uint64_t& value) { return setRawX1(value); }
            void SetRawX2(const uint64_t& value) { return setRawX2(value); }
            const NKikimrCapnProto_::TActorId::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TExecTimeStats {
        struct Reader : private NKikimrCapnProto_::TExecTimeStats::Reader {
            Reader(NKikimrCapnProto_::TExecTimeStats::Reader r) : NKikimrCapnProto_::TExecTimeStats::Reader(r) {}
            Reader() = default;
            uint64_t GetSubmitTimestamp() const { return getSubmitTimestamp(); }
            uint64_t GetInSenderQueue() const { return getInSenderQueue(); }
            uint64_t GetReceivedTimestamp() const { return getReceivedTimestamp(); }
            uint64_t GetTotal() const { return getTotal(); }
            uint64_t GetInQueue() const { return getInQueue(); }
            uint64_t GetExecution() const { return getExecution(); }
            uint64_t GetHugeWriteTime() const { return getHugeWriteTime(); }
            bool HasSubmitTimestamp() const { return getSubmitTimestamp() != 0; }
            bool HasInSenderQueue() const { return getInSenderQueue() != 0; }
            bool HasReceivedTimestamp() const { return getReceivedTimestamp() != 0; }
            bool HasTotal() const { return getTotal() != 0; }
            bool HasInQueue() const { return getInQueue() != 0; }
            bool HasExecution() const { return getExecution() != 0; }
            bool HasHugeWriteTime() const { return getHugeWriteTime() != 0; }
            const NKikimrCapnProto_::TExecTimeStats::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TExecTimeStats::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TExecTimeStats::Builder b) : NKikimrCapnProto_::TExecTimeStats::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetSubmitTimestamp(const uint64_t& value) { return setSubmitTimestamp(value); }
            void SetInSenderQueue(const uint64_t& value) { return setInSenderQueue(value); }
            void SetReceivedTimestamp(const uint64_t& value) { return setReceivedTimestamp(value); }
            void SetTotal(const uint64_t& value) { return setTotal(value); }
            void SetInQueue(const uint64_t& value) { return setInQueue(value); }
            void SetExecution(const uint64_t& value) { return setExecution(value); }
            void SetHugeWriteTime(const uint64_t& value) { return setHugeWriteTime(value); }
            const NKikimrCapnProto_::TExecTimeStats::Builder& GetCapnpBase() const { return *this; }
        };
    };

    enum class EStatus {
        Unknown,
        Success,
        WindowUpdate,
        Processed,
        IncorrectMsgId,
        HighWatermarkOverflow,
    };

    struct TWindowFeedback {
        struct Reader : private NKikimrCapnProto_::TWindowFeedback::Reader {
            Reader(NKikimrCapnProto_::TWindowFeedback::Reader r) : NKikimrCapnProto_::TWindowFeedback::Reader(r) {}
            Reader() = default;
            uint64_t GetActualWindowSize() const { return getActualWindowSize(); }
            uint64_t GetMaxWindowSize() const { return getMaxWindowSize(); }
            TMessageId::Reader GetExpectedMsgId() const { return getExpectedMsgId(); }
            TMessageId::Reader GetFailedMsgId() const { return getFailedMsgId(); }
            EStatus GetStatus() const { return static_cast<EStatus>(static_cast<size_t>(getStatus()) - 1); }
            bool HasExpectedMsgId() const { return hasExpectedMsgId(); }
            bool HasFailedMsgId() const { return hasFailedMsgId(); }
            bool HasStatus() const { return getStatus() != NKikimrCapnProto_::EStatus::NOT_SET; }
            bool HasActualWindowSize() const { return getActualWindowSize() != 0; }
            bool HasMaxWindowSize() const { return getMaxWindowSize() != 0; }
            const NKikimrCapnProto_::TWindowFeedback::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TWindowFeedback::Builder, public Reader {
        private:
            using NKikimrCapnProto_::TWindowFeedback::Builder::getExpectedMsgId;
            using NKikimrCapnProto_::TWindowFeedback::Builder::getFailedMsgId;
        public:
            Builder(NKikimrCapnProto_::TWindowFeedback::Builder b) : NKikimrCapnProto_::TWindowFeedback::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetActualWindowSize(const uint64_t& value) { return setActualWindowSize(value); }
            void SetMaxWindowSize(const uint64_t& value) { return setMaxWindowSize(value); }
            void SetExpectedMsgId(const TMessageId::Reader& value) { return setExpectedMsgId(value.GetCapnpBase()); }
            void SetFailedMsgId(const TMessageId::Reader& value) { return setFailedMsgId(value.GetCapnpBase()); }
            void SetStatus(const EStatus& value) { return setStatus(static_cast<NKikimrCapnProto_::EStatus>(static_cast<size_t>(value) + 1)); }
            TMessageId::Builder MutableExpectedMsgId() { return getExpectedMsgId(); }
            TMessageId::Builder MutableFailedMsgId() { return getFailedMsgId(); }
            const NKikimrCapnProto_::TWindowFeedback::Builder& GetCapnpBase() const { return *this; }
        };
    };

    enum class EVDiskQueueId {
        Unknown,
        PutTabletLog,
        PutAsyncBlob,
        PutUserData,
        GetAsyncRead,
        GetFastRead,
        GetDiscover,
        GetLowRead,
        Begin,
        End,
    };

    enum class EVDiskInternalQueueId {
        IntUnknown,
        IntBegin,
        IntGetAsync,
        IntGetFast,
        IntPutLog,
        IntPutHugeForeground,
        IntPutHugeBackground,
        IntGetDiscover,
        IntLowRead,
        IntEnd,
    };

    struct TVDiskCostSettings {
        struct Reader : private NKikimrCapnProto_::TVDiskCostSettings::Reader {
            Reader(NKikimrCapnProto_::TVDiskCostSettings::Reader r) : NKikimrCapnProto_::TVDiskCostSettings::Reader(r) {}
            Reader() = default;
            uint64_t GetSeekTimeUs() const { return getSeekTimeUs(); }
            uint64_t GetReadSpeedBps() const { return getReadSpeedBps(); }
            uint64_t GetWriteSpeedBps() const { return getWriteSpeedBps(); }
            uint64_t GetReadBlockSize() const { return getReadBlockSize(); }
            uint64_t GetWriteBlockSize() const { return getWriteBlockSize(); }
            uint32_t GetMinREALHugeBlobInBytes() const { return getMinREALHugeBlobInBytes(); }
            bool HasSeekTimeUs() const { return getSeekTimeUs() != 0; }
            bool HasReadSpeedBps() const { return getReadSpeedBps() != 0; }
            bool HasWriteSpeedBps() const { return getWriteSpeedBps() != 0; }
            bool HasReadBlockSize() const { return getReadBlockSize() != 0; }
            bool HasWriteBlockSize() const { return getWriteBlockSize() != 0; }
            bool HasMinREALHugeBlobInBytes() const { return getMinREALHugeBlobInBytes() != 0; }
            const NKikimrCapnProto_::TVDiskCostSettings::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TVDiskCostSettings::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TVDiskCostSettings::Builder b) : NKikimrCapnProto_::TVDiskCostSettings::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetSeekTimeUs(const uint64_t& value) { return setSeekTimeUs(value); }
            void SetReadSpeedBps(const uint64_t& value) { return setReadSpeedBps(value); }
            void SetWriteSpeedBps(const uint64_t& value) { return setWriteSpeedBps(value); }
            void SetReadBlockSize(const uint64_t& value) { return setReadBlockSize(value); }
            void SetWriteBlockSize(const uint64_t& value) { return setWriteBlockSize(value); }
            void SetMinREALHugeBlobInBytes(const uint32_t& value) { return setMinREALHugeBlobInBytes(value); }
            const NKikimrCapnProto_::TVDiskCostSettings::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TMsgQoS {
        struct Reader : private NKikimrCapnProto_::TMsgQoS::Reader {
            Reader(NKikimrCapnProto_::TMsgQoS::Reader r) : NKikimrCapnProto_::TMsgQoS::Reader(r) {}
            Reader() = default;
            uint32_t GetDeadlineSeconds() const { return getDeadlineSeconds(); }
            uint64_t GetCost() const { return getCost(); }
            bool GetSendMeCostSettings() const { return getSendMeCostSettings(); }
            uint32_t GetProxyNodeId() const { return getProxyNodeId(); }
            uint32_t GetReplVDiskId() const { return getReplVDiskId(); }
            uint64_t GetVDiskLoadId() const { return getVDiskLoadId(); }
            uint32_t GetVPatchVDiskId() const { return getVPatchVDiskId(); }
            TMessageId::Reader GetMsgId() const { return getMsgId(); }
            TVDiskCostSettings::Reader GetCostSettings() const { return getCostSettings(); }
            TWindowFeedback::Reader GetWindow() const { return getWindow(); }
            TExecTimeStats::Reader GetExecTimeStats() const { return getExecTimeStats(); }
            TActorId::Reader GetSenderActorId() const { return getSenderActorId(); }
            EVDiskQueueId GetExtQueueId() const { return static_cast<EVDiskQueueId>(static_cast<size_t>(getExtQueueId()) - 1); }
            EVDiskInternalQueueId GetIntQueueId() const { return static_cast<EVDiskInternalQueueId>(static_cast<size_t>(getIntQueueId()) - 1); }
            bool HasMsgId() const { return hasMsgId(); }
            bool HasCostSettings() const { return hasCostSettings(); }
            bool HasWindow() const { return hasWindow(); }
            bool HasExecTimeStats() const { return hasExecTimeStats(); }
            bool HasSenderActorId() const { return hasSenderActorId(); }
            bool HasExtQueueId() const { return getExtQueueId() != NKikimrCapnProto_::EVDiskQueueId::NOT_SET; }
            bool HasIntQueueId() const { return getIntQueueId() != NKikimrCapnProto_::EVDiskInternalQueueId::NOT_SET; }
            bool HasDeadlineSeconds() const { return getDeadlineSeconds() != 0; }
            bool HasCost() const { return getCost() != 0; }
            bool HasSendMeCostSettings() const { return getSendMeCostSettings() != 0; }
            bool HasProxyNodeId() const { return getProxyNodeId() != 0; }
            bool HasReplVDiskId() const { return getReplVDiskId() != 0; }
            bool HasVDiskLoadId() const { return getVDiskLoadId() != 0; }
            bool HasVPatchVDiskId() const { return getVPatchVDiskId() != 0; }
            const NKikimrCapnProto_::TMsgQoS::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TMsgQoS::Builder, public Reader {
        private:
            using NKikimrCapnProto_::TMsgQoS::Builder::getMsgId;
            using NKikimrCapnProto_::TMsgQoS::Builder::getCostSettings;
            using NKikimrCapnProto_::TMsgQoS::Builder::getWindow;
            using NKikimrCapnProto_::TMsgQoS::Builder::getExecTimeStats;
            using NKikimrCapnProto_::TMsgQoS::Builder::getSenderActorId;
        public:
            Builder(NKikimrCapnProto_::TMsgQoS::Builder b) : NKikimrCapnProto_::TMsgQoS::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetDeadlineSeconds(const uint32_t& value) { return setDeadlineSeconds(value); }
            void SetCost(const uint64_t& value) { return setCost(value); }
            void SetSendMeCostSettings(const bool& value) { return setSendMeCostSettings(value); }
            void SetProxyNodeId(const uint32_t& value) { return setProxyNodeId(value); }
            void SetReplVDiskId(const uint32_t& value) { return setReplVDiskId(value); }
            void SetVDiskLoadId(const uint64_t& value) { return setVDiskLoadId(value); }
            void SetVPatchVDiskId(const uint32_t& value) { return setVPatchVDiskId(value); }
            void SetMsgId(const TMessageId::Reader& value) { return setMsgId(value.GetCapnpBase()); }
            void SetCostSettings(const TVDiskCostSettings::Reader& value) { return setCostSettings(value.GetCapnpBase()); }
            void SetWindow(const TWindowFeedback::Reader& value) { return setWindow(value.GetCapnpBase()); }
            void SetExecTimeStats(const TExecTimeStats::Reader& value) { return setExecTimeStats(value.GetCapnpBase()); }
            void SetSenderActorId(const TActorId::Reader& value) { return setSenderActorId(value.GetCapnpBase()); }
            void SetExtQueueId(const EVDiskQueueId& value) { return setExtQueueId(static_cast<NKikimrCapnProto_::EVDiskQueueId>(static_cast<size_t>(value) + 1)); }
            void SetIntQueueId(const EVDiskInternalQueueId& value) { return setIntQueueId(static_cast<NKikimrCapnProto_::EVDiskInternalQueueId>(static_cast<size_t>(value) + 1)); }
            TMessageId::Builder MutableMsgId() { return getMsgId(); }
            TVDiskCostSettings::Builder MutableCostSettings() { return getCostSettings(); }
            TWindowFeedback::Builder MutableWindow() { return getWindow(); }
            TExecTimeStats::Builder MutableExecTimeStats() { return getExecTimeStats(); }
            TActorId::Builder MutableSenderActorId() { return getSenderActorId(); }
            const NKikimrCapnProto_::TMsgQoS::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TVDiskID {
        struct Reader : private NKikimrCapnProto_::TVDiskID::Reader {
            Reader(NKikimrCapnProto_::TVDiskID::Reader r) : NKikimrCapnProto_::TVDiskID::Reader(r) {}
            Reader() = default;
            uint32_t GetGroupID() const { return getGroupID(); }
            uint32_t GetGroupGeneration() const { return getGroupGeneration(); }
            uint32_t GetRing() const { return getRing(); }
            uint32_t GetDomain() const { return getDomain(); }
            uint32_t GetVDisk() const { return getVDisk(); }
            bool HasGroupID() const { return getGroupID() != 0; }
            bool HasGroupGeneration() const { return getGroupGeneration() != 0; }
            bool HasRing() const { return getRing() != 0; }
            bool HasDomain() const { return getDomain() != 0; }
            bool HasVDisk() const { return getVDisk() != 0; }
            const NKikimrCapnProto_::TVDiskID::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TVDiskID::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TVDiskID::Builder b) : NKikimrCapnProto_::TVDiskID::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetGroupID(const uint32_t& value) { return setGroupID(value); }
            void SetGroupGeneration(const uint32_t& value) { return setGroupGeneration(value); }
            void SetRing(const uint32_t& value) { return setRing(value); }
            void SetDomain(const uint32_t& value) { return setDomain(value); }
            void SetVDisk(const uint32_t& value) { return setVDisk(value); }
            const NKikimrCapnProto_::TVDiskID::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TLogoBlobID {
        struct Reader : private NKikimrCapnProto_::TLogoBlobID::Reader {
            Reader(NKikimrCapnProto_::TLogoBlobID::Reader r) : NKikimrCapnProto_::TLogoBlobID::Reader(r) {}
            Reader() = default;
            int64_t GetRawX1() const { return getRawX1(); }
            int64_t GetRawX2() const { return getRawX2(); }
            int64_t GetRawX3() const { return getRawX3(); }
            bool HasRawX1() const { return getRawX1() != 0; }
            bool HasRawX2() const { return getRawX2() != 0; }
            bool HasRawX3() const { return getRawX3() != 0; }
            const NKikimrCapnProto_::TLogoBlobID::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TLogoBlobID::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TLogoBlobID::Builder b) : NKikimrCapnProto_::TLogoBlobID::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetRawX1(const int64_t& value) { return setRawX1(value); }
            void SetRawX2(const int64_t& value) { return setRawX2(value); }
            void SetRawX3(const int64_t& value) { return setRawX3(value); }
            const NKikimrCapnProto_::TLogoBlobID::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TRangeQuery {
        struct Reader : private NKikimrCapnProto_::TRangeQuery::Reader {
            Reader(NKikimrCapnProto_::TRangeQuery::Reader r) : NKikimrCapnProto_::TRangeQuery::Reader(r) {}
            Reader() = default;
            uint64_t GetCookie() const { return getCookie(); }
            uint32_t GetMaxResults() const { return getMaxResults(); }
            TLogoBlobID::Reader GetFrom() const { return getFrom(); }
            TLogoBlobID::Reader GetTo() const { return getTo(); }
            bool HasFrom() const { return hasFrom(); }
            bool HasTo() const { return hasTo(); }
            bool HasCookie() const { return getCookie() != 0; }
            bool HasMaxResults() const { return getMaxResults() != 0; }
            const NKikimrCapnProto_::TRangeQuery::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TRangeQuery::Builder, public Reader {
        private:
            using NKikimrCapnProto_::TRangeQuery::Builder::getFrom;
            using NKikimrCapnProto_::TRangeQuery::Builder::getTo;
        public:
            Builder(NKikimrCapnProto_::TRangeQuery::Builder b) : NKikimrCapnProto_::TRangeQuery::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetCookie(const uint64_t& value) { return setCookie(value); }
            void SetMaxResults(const uint32_t& value) { return setMaxResults(value); }
            void SetFrom(const TLogoBlobID::Reader& value) { return setFrom(value.GetCapnpBase()); }
            void SetTo(const TLogoBlobID::Reader& value) { return setTo(value.GetCapnpBase()); }
            TLogoBlobID::Builder MutableFrom() { return getFrom(); }
            TLogoBlobID::Builder MutableTo() { return getTo(); }
            const NKikimrCapnProto_::TRangeQuery::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TExtremeQuery {
        struct Reader : private NKikimrCapnProto_::TExtremeQuery::Reader {
            Reader(NKikimrCapnProto_::TExtremeQuery::Reader r) : NKikimrCapnProto_::TExtremeQuery::Reader(r) {}
            Reader() = default;
            uint64_t GetShift() const { return getShift(); }
            uint64_t GetSize() const { return getSize(); }
            uint64_t GetCookie() const { return getCookie(); }
            TLogoBlobID::Reader GetId() const { return getId(); }
            bool HasId() const { return hasId(); }
            bool HasShift() const { return getShift() != 0; }
            bool HasSize() const { return getSize() != 0; }
            bool HasCookie() const { return getCookie() != 0; }
            const NKikimrCapnProto_::TExtremeQuery::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TExtremeQuery::Builder, public Reader {
        private:
            using NKikimrCapnProto_::TExtremeQuery::Builder::getId;
        public:
            Builder(NKikimrCapnProto_::TExtremeQuery::Builder b) : NKikimrCapnProto_::TExtremeQuery::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetShift(const uint64_t& value) { return setShift(value); }
            void SetSize(const uint64_t& value) { return setSize(value); }
            void SetCookie(const uint64_t& value) { return setCookie(value); }
            void SetId(const TLogoBlobID::Reader& value) { return setId(value.GetCapnpBase()); }
            TLogoBlobID::Builder MutableId() { return getId(); }
            const NKikimrCapnProto_::TExtremeQuery::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TTabletData {
        struct Reader : private NKikimrCapnProto_::TTabletData::Reader {
            Reader(NKikimrCapnProto_::TTabletData::Reader r) : NKikimrCapnProto_::TTabletData::Reader(r) {}
            Reader() = default;
            uint64_t GetId() const { return getId(); }
            uint32_t GetGeneration() const { return getGeneration(); }
            bool HasId() const { return getId() != 0; }
            bool HasGeneration() const { return getGeneration() != 0; }
            const NKikimrCapnProto_::TTabletData::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private NKikimrCapnProto_::TTabletData::Builder, public Reader {
        private:

        public:
            Builder(NKikimrCapnProto_::TTabletData::Builder b) : NKikimrCapnProto_::TTabletData::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            void SetId(const uint64_t& value) { return setId(value); }
            void SetGeneration(const uint32_t& value) { return setGeneration(value); }
            const NKikimrCapnProto_::TTabletData::Builder& GetCapnpBase() const { return *this; }
        };
    };

    struct TEvVGet {
        struct Reader : private NKikimrCapnProto_::TEvVGet::Reader {
        private:
            std::optional<capnp::PackedMessageReader> messageReader;
        protected:
            std::vector<TExtremeQuery::Reader> elements;
        public:
            Reader(NKikimrCapnProto_::TEvVGet::Reader r) : NKikimrCapnProto_::TEvVGet::Reader(r) {}
            Reader() = default;

            bool HasExtremeQueries() const { return !elements.empty(); }
            TExtremeQuery::Reader GetExtremeQueries(int idx) const { return elements[idx]; }
            const std::vector<TExtremeQuery::Reader>& GetExtremeQueries() const {
                return elements;
            }
            size_t ExtremeQueriesSize() const { return elements.size(); }

            bool GetNotifyIfNotReady() const { return getNotifyIfNotReady(); }
            bool GetShowInternals() const { return getShowInternals(); }
            uint64_t GetCookie() const { return getCookie(); }
            bool GetIndexOnly() const { return getIndexOnly(); }
            bool GetSuppressBarrierCheck() const { return getSuppressBarrierCheck(); }
            uint64_t GetTabletId() const { return getTabletId(); }
            bool GetAcquireBlockedGeneration() const { return getAcquireBlockedGeneration(); }
            uint32_t GetForceBlockedGeneration() const { return getForceBlockedGeneration(); }
            std::string GetSnapshotId() const { return {reinterpret_cast<const char*>(getSnapshotId().begin()), getSnapshotId().size()}; }
            TRangeQuery::Reader GetRangeQuery() const { return getRangeQuery(); }
            TVDiskID::Reader GetVDiskID() const { return getVDiskID(); }
            TMsgQoS::Reader GetMsgQoS() const { return getMsgQoS(); }
            TTimestamps::Reader GetTimestamps() const { return getTimestamps(); }
            TTabletData::Reader GetReaderTabletData() const { return getReaderTabletData(); }
            TTabletData::Reader GetForceBlockTabletData() const { return getForceBlockTabletData(); }
            EGetHandleClass GetHandleClass() const { return static_cast<EGetHandleClass>(static_cast<size_t>(getHandleClass()) - 1); }
            bool HasRangeQuery() const { return hasRangeQuery(); }
            bool HasVDiskID() const { return hasVDiskID(); }
            bool HasMsgQoS() const { return hasMsgQoS(); }
            bool HasTimestamps() const { return hasTimestamps(); }
            bool HasReaderTabletData() const { return hasReaderTabletData(); }
            bool HasForceBlockTabletData() const { return hasForceBlockTabletData(); }
            bool HasHandleClass() const { return getHandleClass() != NKikimrCapnProto_::EGetHandleClass::NOT_SET; }
            bool HasNotifyIfNotReady() const { return getNotifyIfNotReady() != 0; }
            bool HasShowInternals() const { return getShowInternals() != 0; }
            bool HasCookie() const { return getCookie() != 0; }
            bool HasIndexOnly() const { return getIndexOnly() != 0; }
            bool HasSuppressBarrierCheck() const { return getSuppressBarrierCheck() != 0; }
            bool HasTabletId() const { return getTabletId() != 0; }
            bool HasAcquireBlockedGeneration() const { return getAcquireBlockedGeneration() != 0; }
            bool HasForceBlockedGeneration() const { return getForceBlockedGeneration() != 0; }
            bool HasSnapshotId() const { return getSnapshotId() != 0; }
            const NKikimrCapnProto_::TEvVGet::Reader& GetCapnpBase() const { return *this; }
        };

        struct Builder : private capnp::MallocMessageBuilder, private NKikimrCapnProto_::TEvVGet::Builder, public Reader {
        private:
            using NKikimrCapnProto_::TEvVGet::Builder::getRangeQuery;
            using NKikimrCapnProto_::TEvVGet::Builder::getVDiskID;
            using NKikimrCapnProto_::TEvVGet::Builder::getMsgQoS;
            using NKikimrCapnProto_::TEvVGet::Builder::getTimestamps;
            using NKikimrCapnProto_::TEvVGet::Builder::getReaderTabletData;
            using NKikimrCapnProto_::TEvVGet::Builder::getForceBlockTabletData;
            using NKikimrCapnProto_::TEvVGet::Builder::totalSize;
        public:
            Builder() : NKikimrCapnProto_::TEvVGet::Builder(initRoot<NKikimrCapnProto_::TEvVGet>()), Reader(asReader()) {}
            Builder(NKikimrCapnProto_::TEvVGet::Builder b) : NKikimrCapnProto_::TEvVGet::Builder(b), Reader(b.asReader()) {}
            Builder* operator->() { return this; }
            Builder& operator*() { return *this; }

            TExtremeQuery::Builder AddExtremeQueries() {
                auto orphan = getOrphanage().newOrphan<NKikimrCapnProto_::TExtremeQuery>();
                elements.emplace_back(orphan.getReader());
                return orphan.get();
            }

            int ByteSize() const {
                return totalSize().wordCount * 8;
            }

            std::string ShortDebugString() const {
                return "ShortDebugString";
            }

            std::string GetTypeName() const {
                return "TEvVGet";
            }

            bool SerializeToZeroCopyStream(NProtoBuf::io::ZeroCopyOutputStream *output) const {
                NKikimrCapnProto_::TEvVGet::Builder b(*this);
                auto interviews = b.initExtremeQueries(elements.size());
                for (size_t i = 0; i != elements.size(); ++i) {
                    interviews.setWithCaveats(i, GetExtremeQueries(i).GetCapnpBase());
                }
                kj::VectorOutputStream stream;
                capnp::writePackedMessage(stream, *this);
                output->WriteAliasedRaw(stream.getArray().begin(), stream.getArray().size());
                return true;
            }

            void SetNotifyIfNotReady(const bool& value) { return setNotifyIfNotReady(value); }
            void SetShowInternals(const bool& value) { return setShowInternals(value); }
            void SetCookie(const uint64_t& value) { return setCookie(value); }
            void SetIndexOnly(const bool& value) { return setIndexOnly(value); }
            void SetSuppressBarrierCheck(const bool& value) { return setSuppressBarrierCheck(value); }
            void SetTabletId(const uint64_t& value) { return setTabletId(value); }
            void SetAcquireBlockedGeneration(const bool& value) { return setAcquireBlockedGeneration(value); }
            void SetForceBlockedGeneration(const uint32_t& value) { return setForceBlockedGeneration(value); }
            void SetSnapshotId(const std::string& value) { return setSnapshotId({reinterpret_cast<const unsigned char*>(value.data()), value.size()}); }
            void SetRangeQuery(const TRangeQuery::Reader& value) { return setRangeQuery(value.GetCapnpBase()); }
            void SetVDiskID(const TVDiskID::Reader& value) { return setVDiskID(value.GetCapnpBase()); }
            void SetMsgQoS(const TMsgQoS::Reader& value) { return setMsgQoS(value.GetCapnpBase()); }
            void SetTimestamps(const TTimestamps::Reader& value) { return setTimestamps(value.GetCapnpBase()); }
            void SetReaderTabletData(const TTabletData::Reader& value) { return setReaderTabletData(value.GetCapnpBase()); }
            void SetForceBlockTabletData(const TTabletData::Reader& value) { return setForceBlockTabletData(value.GetCapnpBase()); }
            void SetHandleClass(const EGetHandleClass& value) { return setHandleClass(static_cast<NKikimrCapnProto_::EGetHandleClass>(static_cast<size_t>(value) + 1)); }
            TRangeQuery::Builder MutableRangeQuery() { return getRangeQuery(); }
            TVDiskID::Builder MutableVDiskID() { return getVDiskID(); }
            TMsgQoS::Builder MutableMsgQoS() { return getMsgQoS(); }
            TTimestamps::Builder MutableTimestamps() { return getTimestamps(); }
            TTabletData::Builder MutableReaderTabletData() { return getReaderTabletData(); }
            TTabletData::Builder MutableForceBlockTabletData() { return getForceBlockTabletData(); }
            const NKikimrCapnProto_::TEvVGet::Builder& GetCapnpBase() const { return *this; }
        };
    };
};
