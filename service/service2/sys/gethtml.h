#ifndef EXODUSDLFUNC_GETHTML_H
#define EXODUSDLFUNC_GETHTML_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_gethtml;

//a member function with the right arguments, returning a var or void
var gethtml(in mode, out html, in companycode0="")
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_gethtml.pmemberfunction_==NULL)
  efb_gethtml.init("gethtml","exodusprogrambasecreatedelete_",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,out,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 return CALLMEMBERFUNCTION(*(efb_gethtml.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_gethtml.pmemberfunction_)))
  (mode,html,companycode0);

}
#endif
