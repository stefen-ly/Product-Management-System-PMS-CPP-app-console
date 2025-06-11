#include "product.hpp"
#include "order.hpp"
#include "tabulate/table.hpp"

using namespace std;
using namespace tabulate;

Product::Product(string id, string n, string c, double bp, double sp, 
                int q, string e, string cd, double tp, string s, string sz)
    : id(id), name(n), categoryUuid(c), basePrice(bp), sellPrice(sp), quantity(q), 
      expirationDate(e), creationDate(cd), totalPrice(tp), status(s), size(sz) {}

