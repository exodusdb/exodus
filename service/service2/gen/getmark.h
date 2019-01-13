//generated by exodus "compile getmark"
//#ifndef EXODUSDLFUNC_GETMARK_H
#define EXODUSDLFUNC_GETMARK_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_getmark;
class efb_getmark : private ExodusFunctorBase
{
public:

efb_getmark(MvEnvironment& mv) : ExodusFunctorBase("getmark", "exodusprogrambasecreatedelete_", mv) {}

efb_getmark& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, in html, io mark)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_getmark.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_getmark.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,html,mark);

}

};
efb_getmark getmark{mv};
//#endif
