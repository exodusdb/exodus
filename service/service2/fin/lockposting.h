//generated by exodus "compile ../fin/lockposting"
//#ifndef EXODUSDLFUNC_LOCKPOSTING_H
#define EXODUSDLFUNC_LOCKPOSTING_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_lockposting;
class efb_lockposting : private ExodusFunctorBase
{
public:

efb_lockposting(MvEnvironment& mv) : ExodusFunctorBase("lockposting", "exodusprogrambasecreatedelete_", mv) {}

efb_lockposting& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, io locklist, in ntries, io msg)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_lockposting.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_lockposting.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,locklist,ntries,msg);

}

};
efb_lockposting lockposting{mv};
//#endif
