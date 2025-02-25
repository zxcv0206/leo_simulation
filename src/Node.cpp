#include "Node.h"

Node::Node(int id, int timeslot, int type, long long capacity)
    :id(id), timeslot(timeslot), type(type), capacity(capacity), used_capacity(0){
}

int Node::getId() const {
    return id;
}
int Node::getTimeslot() const {
    return timeslot;
}
int Node::getType() const {
    return type;
}
long long Node::getCapacity()const{
    return capacity;
}
long long Node::getUsedCapacity()const{
    return used_capacity;
}

void Node::dataIn(Data* data){
    int grid_id = data->getGrid();
    if(data->getType() == Data::TREE) {
        trees[grid_id] = data;
    } else {
        raw_datas[grid_id].push(data);
    }
    
    used_capacity += data->getSize();
}

void Node::dataOut(Data* data){
    int grid_id = data->getGrid();
    if(data->getType() == Data::TREE) {
        trees.erase(grid_id);
    } else {
        raw_datas[grid_id].push(data);
    }
    
    used_capacity += data->getSize();
}



User::User(int id, int grid, int timeslot)
    : Node(id, timeslot, Node::USER), grid(grid){}

int User::getGrid() const {
    return grid;
}
Data* User::getRawData() {
    if (raw_datas.find(grid) == raw_datas.end()) return nullptr;
    else return raw_datas[grid].front();
}


string User::getName() const {
    return "User[ id" + to_string(id) + ", grid" + to_string(grid) + ", t" + to_string(timeslot) + " ]";
}

Satellite::Satellite(int id, int timeslot, long long storage)
    :Node(id, timeslot, Node::SATELLITE, storage){}

string Satellite::getName() const {
    return "Satellite[ id" + to_string(id) + ", t" + to_string(timeslot) + " ]";

}