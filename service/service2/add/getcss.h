//generated by exodus "compile ../add/getcss"
//#ifndef EXODUSDLFUNC_GETCSS_H
#define EXODUSDLFUNC_GETCSS_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_getcss;
class efb_getcss : private ExodusFunctorBase
{
public:

efb_getcss(MvEnvironment& mv) : ExodusFunctorBase("getcss", "exodusprogrambasecreatedelete_", mv) {}

efb_getcss& operator=(const var& newlibraryname) {
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
 //return CALLMEMBERFUNCTION(*(efb_getcss.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_getcss.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (css,version);

}

var operator() ( in css=var(), in version="") {
 var css_io;
 if (css.assigned()) css_io=css;
 return operator()(css_io,version);
}

};
efb_getcss getcss{mv};
//#endif
