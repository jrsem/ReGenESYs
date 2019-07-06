/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Decide.cpp
 * Author: rafael.luiz.cancian
 *
 * Created on 22 de Maio de 2019, 18:41
 */

#include "Decide.h"
#include "Model.h"
#include <string>

Decide::Decide(Model* model) : ModelComponent(model, Util::TypeOf<Decide>()) {
}

Decide::Decide(const Decide& orig) : ModelComponent(orig) {
}

Decide::~Decide() {
}

std::string Decide::show() {
    return ModelComponent::show() + "";
}

List<std::string>* Decide::getConditions() const {
    return _conditions;
}

ModelComponent* Decide::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
    Decide* newComponent = new Decide(model);
    try {
	newComponent->_loadInstance(fields);
    } catch (const std::exception& e) {

    }
    return newComponent;
}

void Decide::_execute(Entity* entity) {
    //_model->getTraceManager()->trace(Util::TraceLevel::report, "I'm just a Decide model and I'll just send the entity forward");
    //string* exp = this->_conditions->first();
    std::string let;
    std::list<std::string>* lets = this->_conditions->getList();
    for (std::list<std::string>::iterator it = lets->begin(); it != lets->end(); it++) {
	       let = (*it);
       }
    std::string err;
    bool success;
    double value = _model->parseExpression(let,&success,&err);
    bool s = &success;
    //std::cout <<"succes checkExpression=" <<success;
    _model->getTraceManager()->trace(Util::TraceLevel::report, "success checkExpression="+std::to_string(value));
  
    if(value==1)

           this->_model->sendEntityToComponent(entity, this->getNextComponents()->front(), 0.0);
       else{

          //auto it1 = std::next(comps->begin(),1);
           this->_model->sendEntityToComponent(entity, this->getNextComponents()->getAtRank(1), 0.0);
        }
}

bool Decide::_loadInstance(std::map<std::string, std::string>* fields) {
    bool res = ModelComponent::_loadInstance(fields);
    if (res) {
    }
    return res;
}

void Decide::_initBetweenReplications() {
}

std::map<std::string, std::string>* Decide::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
    return fields;
}

bool Decide::_check(std::string* errorMessage) {
    return true;
}

PluginInformation* Decide::GetPluginInformation(){
    return new PluginInformation(Util::TypeOf<Decide>(), &Decide::LoadInstance);
}
