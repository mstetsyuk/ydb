#include "object.h"

namespace NKikimr::NMetadata::NModifications {

Ydb::Table::CreateTableRequest TBaseObject::AddHistoryTableScheme(const Ydb::Table::CreateTableRequest& baseScheme, const TString& tableName) {
    Ydb::Table::CreateTableRequest result = baseScheme;
    result.add_primary_key("historyInstant");
    result.set_path(tableName);
    {
        auto& column = *result.add_columns();
        column.set_name("historyAction");
        column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
    }
    {
        auto& column = *result.add_columns();
        column.set_name("historyUserId");
        column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::STRING);
    }
    {
        auto& column = *result.add_columns();
        column.set_name("historyInstant");
        column.mutable_type()->mutable_optional_type()->mutable_item()->set_type_id(Ydb::Type::UINT64);
    }
    return result;
}

}
