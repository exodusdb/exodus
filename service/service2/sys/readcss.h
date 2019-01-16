//generated by exodus "compile ../sys/readcss"
//#ifndef EXODUSDLFUNC_READCSS_H
#define EXODUSDLFUNC_READCSS_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_readcss;
class efb_readcss : private ExodusFunctorBase
{
public:

efb_readcss(MvEnvironment& mv) : ExodusFunctorBase("readcss", "exodusprogrambasecreatedelete_", mv) {}

efb_readcss& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (io css, in version="")
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
 //return CALLMEMBERFUNCTION(*(efb_readcss.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_readcss.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (css,version);

}

};
efb_readcss readcss{mv};
//#endif
