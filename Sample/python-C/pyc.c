#include    <Python2.7/Python.h>

int main(void)
{
  Py_Initialize();
  PyRun_SimpleString("print 'hello python'");
  Py_Finalize();
  return 0;
}
