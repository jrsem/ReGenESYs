/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Decide.cpp
 * Authors: Junior Semerzier (13106344)
 *          Marcelo José Dias (15205398)
 *          Thomas Fernandes Feijoo (12200662)
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

ModelComponent *Decide::LoadInstance(Model *model, std::map<std::string, std::string> *fields)
{
  Decide *newComponent = new Decide(model);
  try
  {
    newComponent->_loadInstance(fields);
  }
  catch (const std::exception &e)
  {
  }
  return newComponent;
}
/**
 * @brief Basically what it's done here: an iteration throught the conditions, which are strings, checking which ones are true.
 * Each condition string is considered an Expression, and it's parsed with parseExpression(), a method from Model.
 * If the condition is true, parseExpression() returns 1.0. If false, returns 0.0.
 * Also, if a condition is true, we utilize its index to find to which block we will send our entity.
 * 
 * @param entity 
 */
void Decide::_execute(Entity* entity) {

    std::string let;
    std::list<std::string>* lets = this->_conditions->getList();

    double value,attr;
    int index=0;
    bool elseCondition = true; // if it remains true after checking conditions, the else clause of Decide is used
    for (std::list<std::string>::iterator it = lets->begin(); it != lets->end(); it++) {
      let = (*it);
      value = _model->parseExpression(let);
      /* Debug */
      attr = entity->getAttributeValue("Attribute_1");
      _model->getTraceManager()->trace(Util::TraceLevel::report, "Attribute_1="+std::to_string(attr));
      _model->getTraceManager()->trace(Util::TraceLevel::report, "success checkExpression="+std::to_string(value));

      //If expression is true, send entity to the next block
      if(value != 0){
          elseCondition = false;
          this->_model->sendEntityToComponent(entity, this->getNextComponents()->getAtRank(index), 0.0);
          break;
      }
      index++;
    }
      //No condition were met, go to else condition
    if(elseCondition){
      _model->getTraceManager()->trace(Util::TraceLevel::report, "else condition");
      this->_model->sendEntityToComponent(entity, this->getNextComponents()->getAtRank(index), 0.0);  // last() didn't work, so we just use the last index
    }
}

/**
 * @brief Conditions that create a Decide are loaded back from file.
 * 
 * @param fields 
 * @return true 
 * @return false 
 */
bool Decide::_loadInstance(std::map<std::string, std::string> *fields)
{
  bool res = ModelComponent::_loadInstance(fields);
  if (res)
  {
    unsigned int nv = std::stoi((*(fields->find("conditions"))).second);
    for (unsigned int i = 0; i < nv; i++)
    {
      std::string exp = ((*(fields->find("expression" + std::to_string(i)))).second);
      this->_conditions->insert(exp);
    }
  }
  return res;
}

void Decide::_initBetweenReplications() {
}

/**
 * @brief Conditions that create a Decide are organized to be inserted in file.
 * 
 * @return std::map<std::string, std::string>* 
 */
std::map<std::string, std::string>* Decide::_saveInstance() {
    std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
    std::list<std::string>* lets = this->_conditions->getList();
    std::string let;
    fields->emplace("conditions", std::to_string(_conditions->size()));
    unsigned short i = 0;
    for (std::list<std::string>::iterator it = lets->begin(); it != lets->end(); it++) {
      let = (*it);
      fields->emplace("condition" + std::to_string(i), let);
      i++;
    }
    return fields;
}

/**
 * @brief Iteration to check if every condition is ok.
 * 
 * @param errorMessage 
 * @return true 
 * @return false 
 */
bool Decide::_check(std::string* errorMessage) {
    bool resultAll=true;
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
