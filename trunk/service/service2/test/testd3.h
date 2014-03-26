#ifndef EXODUSDLFUNC_TESTD3_H
#define EXODUSDLFUNC_TESTD3_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_testd3;

//a member function with the right arguments, returning a var
var testd3()
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_testd3.pmemberfunction_==NULL)
  efb_testd3.init("testd3","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)();

 //call the shared library object main function with the right args, returning a var
 return CALLMEMBERFUNCTION(*(efb_testd3.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_testd3.pmemberfunction_)))
  ();

}
#endif
