// Copyright (C) 2008-2016 LAAS-CNRS, JRL AIST-CNRS.
//
// This file is part of sot-dynamic.
// sot-dynamic is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// sot-dynamic is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// You should have received a copy of the GNU Lesser General Public License
// along with sot-dynamic.  If not, see <http://www.gnu.org/licenses/>.

#include <Python.h>
#include <sot-dynamic/dynamic.h>
#include <boost/python.hpp>
#include <typeinfo>
#include <cstdio>
#include <pinocchio/python/model.hpp>
#include <pinocchio/python/data.hpp>
#include <pinocchio/python/handler.hpp>


namespace dynamicgraph{
  namespace sot{

    /*    PyObject* getPinocchioModel(PyObject* // self
				,PyObject* args) {
      PyObject* object = NULL;
      void* pointer = NULL;
      
      if (!PyArg_ParseTuple(args, "O", &object))
	return NULL;
      
      if (!PyCObject_Check(object)) {
	PyErr_SetString(PyExc_TypeError,
			"function takes a PyCObject as argument");
	return NULL;
      }
      
      pointer = PyCObject_AsVoidPtr(object);
      Dynamic* dyn_entity = (Dynamic*) pointer;
      
      se3::Model* model_ptr = NULL;
      
      try {
      model_ptr = dyn_entity->m_model;
      se3::python::ModelHandler& _model(& (dyn_entity->m_model));	
      }
      catch (const std::exception& exc) {
      PyErr_SetString(dgpyError, exc.what());			
      return NULL;						
      }								
      catch (const char* s) {								
      PyErr_SetString(dgpyError, s);
      return NULL;
      }
      catch (...) {
	    PyErr_SetString(dgpyError, "Unknown exception");		
	    return NULL;						
	    }
	    //CATCH_ALL_EXCEPTIONS();
	    
	    // Return the pointer to the signal without destructor since the signal
	    // is not owned by the calling object but by the Entity.
	    //return boost::python::incref();
	    return PyCObject_FromVoidPtr((void*)model_ptr, NULL);
    }
    */

    PyObject* setPinocchioModel(PyObject* /* self */,PyObject* args) {
      PyObject* object = NULL;
      PyObject* pyPinocchioObject;
      void* pointer1 = NULL;
      if (!PyArg_ParseTuple(args, "OO", &object, &pyPinocchioObject))
	return NULL;

      if (!PyCObject_Check(object)) {
	PyErr_SetString(PyExc_TypeError,
			"function takes a PyCObject as argument");
	return NULL;
      }

      pointer1 = PyCObject_AsVoidPtr(object);
      Dynamic* dyn_entity = (Dynamic*) pointer1;

      std::string msg("Error in obtaining pinocchio model");
      PyObject* dgpyError =
	PyErr_NewException(const_cast<char*>(msg.c_str()), NULL, NULL);
      
      try {
	se3::python::ModelHandler cppModelHandle = 
	  boost::python::extract<se3::python::ModelHandler>(pyPinocchioObject);
	dyn_entity->m_model = cppModelHandle.ptr();
      }
      catch (const std::exception& exc) {
	PyErr_SetString(dgpyError, exc.what());			
	return NULL;
      }								
      catch (const char* s) {								
	PyErr_SetString(dgpyError, s);
	return NULL;
      }
      catch (...) {
	PyErr_SetString(dgpyError, "Unknown exception");		
	return NULL;						
      }
      // Return the pointer to the signal without destructor since the signal
      // is not owned by the calling object but by the Entity.
      //return boost::python::incref();
      return Py_BuildValue("");
    }

    PyObject* setPinocchioData(PyObject* /* self */,PyObject* args) {
      PyObject* object = NULL;
      PyObject* pyPinocchioObject;
      void* pointer1 = NULL;
      if (!PyArg_ParseTuple(args, "OO", &object, &pyPinocchioObject))
	return NULL;

      if (!PyCObject_Check(object)) {
	PyErr_SetString(PyExc_TypeError,
			"function takes a PyCObject as argument");
	return NULL;
      }

      pointer1 = PyCObject_AsVoidPtr(object);
      Dynamic* dyn_entity = (Dynamic*) pointer1;

      std::string msg("Error in obtaining pinocchio model");
      PyObject* dgpyError =
	PyErr_NewException(const_cast<char*>(msg.c_str()), NULL, NULL);
      
      try {
	se3::python::DataHandler cppDataHandle = 
	  boost::python::extract<se3::python::DataHandler>(pyPinocchioObject);
	dyn_entity->m_data = cppDataHandle.ptr();
      }
      catch (const std::exception& exc) {
	PyErr_SetString(dgpyError, exc.what());			
	return NULL;
      }								
      catch (const char* s) {								
	PyErr_SetString(dgpyError, s);
	return NULL;
      }
      catch (...) {
	PyErr_SetString(dgpyError, "Unknown exception");		
	return NULL;						
      }
      // Return the pointer to the signal without destructor since the signal
      // is not owned by the calling object but by the Entity.
      //return boost::python::incref();
      return Py_BuildValue("");
    }
  }
}
/**
   \brief List of python functions
*/

static PyMethodDef functions[] = {
  /*  {"get_pinocchio_model",  dynamicgraph::sot::getPinocchioModel, METH_VARARGS,
      "Get the pinocchio model as python object"},*/
  {"set_pinocchio_model",  dynamicgraph::sot::setPinocchioModel, METH_VARARGS,
   "Set the model from pinocchio python object"},
  {"set_pinocchio_data",  dynamicgraph::sot::setPinocchioData, METH_VARARGS,
   "Set the data from pinocchio python object"},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initwrap(void)
{
    PyObject *m;

    m = Py_InitModule("wrap", functions);
    if (m == NULL)
        return;
}
