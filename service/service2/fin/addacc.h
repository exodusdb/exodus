//generated by exodus "compile ../fin/addacc"
//#ifndef EXODUSDLFUNC_ADDACC_H
#define EXODUSDLFUNC_ADDACC_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_addacc;
class efb_addacc : private ExodusFunctorBase
{
public:

efb_addacc(MvEnvironment& mv) : ExodusFunctorBase("addacc", "exodusprogrambasecreatedelete_", mv) {}

efb_addacc& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, io ledgercode, in newaccno0, in newaccount, in sortorder, in params0, out msg)
{

 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 //first time link to the shared lib and create/cache an object from it
 //passing current standard variables in mv
 //if (efb_getlang.pmemberfunction_==NULL)
 // efb_getlang.init("getlang","exodusprogrambasecreatedelete_",mv);
 if (this->pmemberfunction_==NULL)
  this->init();

 //define a function type (pExodusProgramBaseMemberFunction)
 //that can call the shared library object member function
 //with the right arguments and returning a var or void
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,in,in,in,in,out);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_addacc.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_addacc.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,ledgercode,newaccno0,newaccount,sortorder,params0,msg);

}

};
efb_addacc addacc{mv};
//#endif
