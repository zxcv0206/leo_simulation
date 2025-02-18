#include "Data.h"
#include "Node.h"

Data::Data(int type, User *user, long long data_cnt)
    :type(type), user(user), data_cnt(data_cnt) {
}

int Data::getType() const {
    return type;
}
User* Data::getUser() const {
    return user;
}
long long Data::getDataCnt() const {
    return data_cnt;
}

int Data::getGrid() const {
    return user->getGrid();
}
long long Data::getSize() const {
    return log(data_cnt);
}

void Data::pathAdd(pair<int,int> link) {
    path.push_back(link);
}