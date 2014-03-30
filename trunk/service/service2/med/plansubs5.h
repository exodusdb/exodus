#ifndef EXODUSDLFUNC_PLANSUBS5_H
#define EXODUSDLFUNC_PLANSUBS5_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_plansubs5;

//a member function with the right arguments, returning a var or void
var plansubs5(in mode)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_plansubs5.pmemberfunction_==NULL)
  efb_plansubs5.init("plansubs5","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_plansubs5.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_plansubs5.pmemberfunction_)))
  (mode);

}
#endif
