#include "Data.h"
#include "Node.h"

Data::Data(string type, long long data_cnt)
    :type(type), data_cnt(data_cnt) {
}

string Data::getType() const {
    return type;
}
long long Data::getDataCnt() const {
    return data_cnt;
}
vector<Link*> Data::getPath() const {
    return path;
}

long long Data::getSize() const {
    return log(data_cnt);
}

void Data::pathAdd(Link* link) {
    path.push_back(link);
}

RawData::RawData(User* user, long long data_cnt)
    :Data(RAWDATA, data_cnt), user(user){
}

User* RawData::getUser() const {
    return user;
}

int RawData::getGrid() const {
    return user->getGrid();
}

string RawData::getName() const { 
    return "RawData[ " + user->getName() + " ]";
}

Tree::Tree(int grid, long long data_cnt)
    :Data(TREE, data_cnt), grid(grid){
}

int Tree::getGrid() const {
    return grid;
}

string Tree::getName() const {
    return "Tree[ grid" + to_string(grid) + ", k" + to_string(data_cnt) + " ]";
}

void Tree::insert(RawData* data) {
    data_cnt++;
}