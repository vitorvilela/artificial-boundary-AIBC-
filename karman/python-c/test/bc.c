#include <Python.h>
#include <stdlib.h>

// If your library name is say libxyz.so and it is located on path say:
// /home/user/myDir
// then to link it to your program:
// g++ -L/home/user/myDir -lxyz myprog.cpp -o myprog

// With main():
// gcc bc.c -Wall -I/usr/include/python2.7 -lpython2.7 -o bc
// Python lib at: /usr/lib/python2.7/config-x86_64-linux-gnu/libpython2.7.so
// ./velt

// Creating mylib:
// gcc -Wall -I/usr/include/python2.7 -lpython2.7 -c *.c
// OR
// gcc -Wall -I/usr/include/python3.5 -L/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu -lpython3.5 -c *.c
// ar -cvq libbc.a bc.o     
// OR
// ar -cvq libbc.a *.o

void  Initialize(){
    Py_Initialize();
}

void Finalize() {
    Py_Finalize();
}


double get_new_ux(double dt, double y, double ux)
{
    
    static int currentTurn;
    
    double bc = 0.;
  
    // Set PYTHONPATH TO working directory in order to load built-in python modules
    setenv("PYTHONPATH", ".", 1);
    
    // Declare C-Python variables
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pResult;
    
    // Initialize the Python Interpreter
    if (currentTurn == 0) {   
        ++currentTurn;
        Initialize();
        atexit(Finalize);
    }
    

    // Build the name object
    pName = PyUnicode_FromString((char*)"bc_model");
 
    // Load the module object
    pModule = PyImport_Import(pName);
    // Clean up
    Py_DECREF(pName);
    
    if (pModule != NULL) {
         
        pFunc = PyObject_GetAttrString(pModule, (char*)"get_new_ux");
        
        if (PyCallable_Check(pFunc)) {
                        
            pArgs = PyTuple_New(3);
            pValue = PyFloat_FromDouble(dt);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 0, pValue);
           
            pValue = PyFloat_FromDouble(y);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 1, pValue);
            
            pValue = PyFloat_FromDouble(ux);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 2, pValue);
                       
            pResult = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pValue);
            Py_DECREF(pArgs);
        
            if (pResult != NULL) {                
                bc = PyFloat_AsDouble(pResult);
                Py_DECREF(pResult);
            }
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
            fprintf(stderr, "Cannot find function \"%s\"\n", "get_new_ux");
            
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
                
    } 
    else {
        
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "bc_model");
        return 1;
        
    }  


    // Finish the Python Interpreter
    //Py_Finalize();

    return bc;
    
}


double get_new_uy(double dt, double y, double uy)
{
    
    static int currentTurn;
    
    double bc = 0.;
  
    // Set PYTHONPATH TO working directory in order to load built-in python modules
    setenv("PYTHONPATH", ".", 1);
    
    // Declare C-Python variables
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pResult;
    
    // Initialize the Python Interpreter
    if (currentTurn == 0) {   
        ++currentTurn;
        Initialize();
        atexit(Finalize);
    }
    

    // Build the name object
    pName = PyUnicode_FromString((char*)"bc_model");
 
    // Load the module object
    pModule = PyImport_Import(pName);
    // Clean up
    Py_DECREF(pName);
    
    if (pModule != NULL) {
         
        pFunc = PyObject_GetAttrString(pModule, (char*)"get_new_uy");
        
        if (PyCallable_Check(pFunc)) {
                        
            pArgs = PyTuple_New(3);
            pValue = PyFloat_FromDouble(dt);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 0, pValue);
           
            pValue = PyFloat_FromDouble(y);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 1, pValue);
            
            pValue = PyFloat_FromDouble(uy);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 2, pValue);
                       
            pResult = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pValue);
            Py_DECREF(pArgs);
        
            if (pResult != NULL) {                
                bc = PyFloat_AsDouble(pResult);
                Py_DECREF(pResult);
            }
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
            fprintf(stderr, "Cannot find function \"%s\"\n", "get_new_uy");
            
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
                
    } 
    else {
        
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "bc_model");
        return 1;
        
    }  


    // Finish the Python Interpreter
    //Py_Finalize();

    return bc;
    
}




double get_new_f(double dt, double y, double f)
{
    
    static int currentTurn;
    
    double bc = 0.;
  
    // Set PYTHONPATH TO working directory in order to load built-in python modules
    setenv("PYTHONPATH", ".", 1);
    
    // Declare C-Python variables
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pResult;
    
    // Initialize the Python Interpreter
    if (currentTurn == 0) {   
        ++currentTurn;
        Initialize();
        atexit(Finalize);
    }
    

    // Build the name object
    pName = PyUnicode_FromString((char*)"bc_model");
 
    // Load the module object
    pModule = PyImport_Import(pName);
    // Clean up
    Py_DECREF(pName);
    
    if (pModule != NULL) {
         
        pFunc = PyObject_GetAttrString(pModule, (char*)"get_new_f");
        
        if (PyCallable_Check(pFunc)) {
                        
            pArgs = PyTuple_New(3);
            pValue = PyFloat_FromDouble(dt);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 0, pValue);
           
            pValue = PyFloat_FromDouble(y);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 1, pValue);
            
            pValue = PyFloat_FromDouble(f);
            if (!pValue) {
                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            PyTuple_SetItem(pArgs, 2, pValue);
                       
            pResult = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pValue);
            Py_DECREF(pArgs);
        
            if (pResult != NULL) {                
                bc = PyFloat_AsDouble(pResult);
                Py_DECREF(pResult);
            }
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
            fprintf(stderr, "Cannot find function \"%s\"\n", "get_new_f");
            
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
                
    } 
    else {
        
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", "bc_model");
        return 1;
        
    }  


    // Finish the Python Interpreter
    //Py_Finalize();

    return bc;
    
}






