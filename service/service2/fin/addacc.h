#ifndef EXODUSDLFUNC_ADDACC_H
#define EXODUSDLFUNC_ADDACC_H

//a member variable/object to cache a pointer/object for the shared library function
ExodusFunctorBase efb_addacc;

//a member function with the right arguments, returning a var
var addacc(in mode, io ledgercode, in newaccno0, in newaccount, in sortorder, in params0, out msg)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 if (efb_addacc.pmemberfunction_==NULL)
  efb_addacc.init("addacc","exodusprogrambasecreatedelete",mv);

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,in,in,in,in,out);

 //call the shared library object main function with the right args, returning a var
 return CALLMEMBERFUNCTION(*(efb_addacc.pobject_),
 ((pExodusProgramBaseMemberFunction) (efb_addacc.pmemberfunction_)))
  (mode,ledgercode,newaccno0,newaccount,sortorder,params0,msg);

}
#endif