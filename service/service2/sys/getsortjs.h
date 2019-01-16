//generated by exodus "compile ../sys/getsortjs"
//#ifndef EXODUSDLFUNC_GETSORTJS_H
#define EXODUSDLFUNC_GETSORTJS_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_getsortjs;
class efb_getsortjs : private ExodusFunctorBase
{
public:

efb_getsortjs(MvEnvironment& mv) : ExodusFunctorBase("getsortjs", "exodusprogrambasecreatedelete_", mv) {}

efb_getsortjs& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (io tx)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_getsortjs.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_getsortjs.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (tx);

}

};
efb_getsortjs getsortjs{mv};
//#endif
