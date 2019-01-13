//generated by exodus "compile htmllib"
//#ifndef EXODUSDLFUNC_HTMLLIB_H
#define EXODUSDLFUNC_HTMLLIB_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_htmllib;
class efb_htmllib : private ExodusFunctorBase
{
public:

efb_htmllib(MvEnvironment& mv) : ExodusFunctorBase("htmllib", "exodusprogrambasecreatedelete_", mv) {}

efb_htmllib& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, io datax, in params0="", in params20="")
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,io,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_htmllib.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_htmllib.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,datax,params0,params20);

}

};
efb_htmllib htmllib{mv};
//#endif
