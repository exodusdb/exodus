//generated by exodus "compile ../med/mediaalerts"
//#ifndef EXODUSDLFUNC_MEDIAALERTS_H
#define EXODUSDLFUNC_MEDIAALERTS_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_mediaalerts;
class efb_mediaalerts : private ExodusFunctorBase
{
public:

efb_mediaalerts(MvEnvironment& mv) : ExodusFunctorBase("mediaalerts", "exodusprogrambasecreatedelete_", mv) {}

efb_mediaalerts& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
}

//a member function with the right arguments, returning a var or void
var operator() (in alerttype0, in runasuser, out tasks, out title, out request, out datax)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,out,out,out,out);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_mediaalerts.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_mediaalerts.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (alerttype0,runasuser,tasks,title,request,datax);

}

};
efb_mediaalerts mediaalerts{mv};
//#endif
