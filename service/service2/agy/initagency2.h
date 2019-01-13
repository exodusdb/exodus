//generated by exodus "compile ../agy/initagency2"
//#ifndef EXODUSDLFUNC_INITAGENCY2_H
#define EXODUSDLFUNC_INITAGENCY2_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_initagency2;
class efb_initagency2 : private ExodusFunctorBase
{
public:

efb_initagency2(MvEnvironment& mv) : ExodusFunctorBase("initagency2", "exodusprogrambasecreatedelete_", mv) {}

efb_initagency2& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (io logtime, in reloading="")
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_initagency2.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_initagency2.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (logtime,reloading);

}

};
efb_initagency2 initagency2{mv};
//#endif
