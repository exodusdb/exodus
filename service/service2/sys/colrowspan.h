//generated by exodus "compile ../sys/colrowspan"
//#ifndef EXODUSDLFUNC_COLROWSPAN_H
#define EXODUSDLFUNC_COLROWSPAN_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_colrowspan;
class efb_colrowspan : private ExodusFunctorBase
{
public:

efb_colrowspan(MvEnvironment& mv) : ExodusFunctorBase("colrowspan", "exodusprogrambasecreatedelete_", mv) {}

efb_colrowspan& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (io colhdg, in thproperties, in nobase0)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(io,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_colrowspan.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_colrowspan.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (colhdg,thproperties,nobase0);

}

var operator() ( in colhdg, in thproperties, in nobase0) {
 var colhdg_io;
 if (colhdg.assigned()) colhdg_io=colhdg;
 return operator()(colhdg_io,thproperties,nobase0);
}

};
efb_colrowspan colrowspan{mv};
//#endif
