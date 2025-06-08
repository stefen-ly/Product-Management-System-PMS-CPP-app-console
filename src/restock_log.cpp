#include "restock_log.hpp"

using namespace std;

RestockLog::RestockLog(string id, string name, int qty, string date)
    : productId(id), productName(name), quantityRestocked(qty), restockDate(date) {}
