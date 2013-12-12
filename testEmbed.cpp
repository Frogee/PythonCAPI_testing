#include <stdlib.h>
#include <iostream>
#include <vector>
#include <Python.h>



int runPython(int argc, char * argv[]) {
	std::cout << "Hello from runPython()" << std::endl;
	
	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgTuple, *pValue, *pXVec, *pYVec;
	
	int i;
	
	if (argc < 3) {
		fprintf(stderr,"Usage: programName pythonFile functionName\n");
		return 1;
	}
	
	Py_Initialize();
	
	// Set the path to include the current directory in case the module is located there. Found from
	// http://stackoverflow.com/questions/7624529/python-c-api-doesnt-load-module
	// and http://stackoverflow.com/questions/7283964/embedding-python-into-c-importing-modules
	PyObject *sys = PyImport_ImportModule("sys");
	PyObject *path = PyObject_GetAttrString(sys, "path");
	PyList_Append(path, PyString_FromString("."));
	
	pName = PyString_FromString(argv[1]);   //Get the name of the module
	pModule = PyImport_Import(pName);     //Get the module
	
	Py_DECREF(pName);
	
	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, argv[2]);   //Get the function by its name
		/* pFunc is a new reference */
		
		if (pFunc && PyCallable_Check(pFunc)) {

			//Set up a tuple that will contain the function arguments. In this case, the
			//function requires two tuples, so we set up a tuple of size 2.
			pArgTuple = PyTuple_New(2);

			//Make some vectors containing the data
		        static const double xarr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14};
			std::vector<double> xvec (xarr, xarr + sizeof(xarr) / sizeof(xarr[0]) );
		        static const double yarr[] = {0,0,1,1,0,0,2,2,0,0,1,1,0,0};
			std::vector<double> yvec (yarr, yarr + sizeof(yarr) / sizeof(yarr[0]) );

			//Transfer the C++ vector to a python tuple
			pXVec = PyTuple_New(xvec.size());	
			for (i = 0; i < xvec.size(); ++i) {
				pValue = PyFloat_FromDouble(xvec[i]);
				if (!pValue) {
					Py_DECREF(pXVec);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert array value\n");
					return 1;
				}
				PyTuple_SetItem(pXVec, i, pValue);
			}

			//Transfer the other C++ vector to a python tuple
			pYVec = PyTuple_New(yvec.size());	
			for (i = 0; i < yvec.size(); ++i) {
				pValue = PyFloat_FromDouble(yvec[i]);
				if (!pValue) {
					Py_DECREF(pYVec);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert array value\n");
					return 1;
				}
				PyTuple_SetItem(pYVec, i, pValue); //
			}

			//Set the argument tuple to contain the two input tuples
			PyTuple_SetItem(pArgTuple, 0, pXVec);
			PyTuple_SetItem(pArgTuple, 1, pYVec);

			//Call the python function
			pValue = PyObject_CallObject(pFunc, pArgTuple);
			
			Py_DECREF(pArgTuple);
			Py_DECREF(pXVec);
			Py_DECREF(pYVec);

			if (pValue != NULL) {
				printf("Result of call: %ld\n", PyInt_AsLong(pValue));
				Py_DECREF(pValue);
			}

			//Some error catching
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr,"Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	Py_Finalize();
	return 0;
}

int main ( int argc, char *argv[] )
{
	std::cout << "Hello from main" << std::endl;
	runPython(argc, argv);
	std::cout << "Program finished" << std::endl;
	return EXIT_SUCCESS;
}
