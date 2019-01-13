//generated by exodus "compile ../fin/getacc"
//#ifndef EXODUSDLFUNC_GETACC_H
#define EXODUSDLFUNC_GETACC_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_getacc;
class efb_getacc : private ExodusFunctorBase
{
public:

efb_getacc(MvEnvironment& mv) : ExodusFunctorBase("getacc", "exodusprogrambasecreatedelete_", mv) {}

efb_getacc& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (io mode, io validaccs, io reqcompanies2, io acctype, io reqledgertype, io getacc2, io inpaccnos)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,io,io,io,io,io,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_getacc.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_getacc.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,validaccs,reqcompanies2,acctype,reqledgertype,getacc2,inpaccnos);

}

};
efb_getacc getacc{mv};
//#endif
