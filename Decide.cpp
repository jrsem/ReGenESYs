/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Decide.cpp
 * Author: marcelo
 *
 * Created on 22 de Junho de 2019, 18:41
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
    std::string let;
    std::list<std::string>* lets = this->_conditions->getList();

    double value,attr;
    int index=0;
    bool elseCondition = true;//indica que vai para a saída else do bloco Decide
    for (std::list<std::string>::iterator it = lets->begin(); it != lets->end(); it++) {
      let = (*it);
      value = _model->parseExpression(let);
      /* Debug */
      attr = entity->getAttributeValue("Attribute_1");
      _model->getTraceManager()->trace(Util::TraceLevel::report, "Attribute_1="+std::to_string(attr));
      _model->getTraceManager()->trace(Util::TraceLevel::report, "success checkExpression="+std::to_string(value));
      /*       */
      //Se expressão retornou true, envia entidade para bloco seguinte.
      if(value != 0){
          elseCondition = false;
          this->_model->sendEntityToComponent(entity, this->getNextComponents()->getAtRank(index), 0.0);
          break;
      }
      index++;
    }
      //se não aceitou nenhuma condição a entidade vai para o else do Decide.
    if(elseCondition){
      _model->getTraceManager()->trace(Util::TraceLevel::report, "else condition");
      //this->_model->sendEntityToComponent(entity, this->getNextComponents()->last(), 0.0);
      this->_model->sendEntityToComponent(entity, this->getNextComponents()->getAtRank(index), 0.0);
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
    bool resultAll;
    std::string let, err;
    std::list<std::string>* lets = this->_conditions->getList();

    for (std::list<std::string>::iterator it = lets->begin(); it != lets->end(); it++) {
      let = (*it);
      resultAll &= _model->checkExpression(let,"Decide Condition "+let,&err);
    }
    return resultAll;
}

PluginInformation* Decide::GetPluginInformation(){
    return new PluginInformation(Util::TypeOf<Decide>(), &Decide::LoadInstance);
}
