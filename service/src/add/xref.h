//generated by exodus "compile ../add/xref"
//#ifndef EXODUSDLFUNC_XREF_H
#define EXODUSDLFUNC_XREF_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_xref;
class efb_xref : private ExodusFunctorBase
{
public:

efb_xref(MvEnvironment& mv) : ExodusFunctorBase("xref", "exodusprogrambasecreatedelete_", mv) {}

efb_xref& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in instr, in sepchars, in stoplist, in mode)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_xref.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_xref.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (instr,sepchars,stoplist,mode);

}

var operator() () {
 var instr_in;
 var sepchars_in;
 var stoplist_in;
 var mode_in;
 return operator()(instr_in, sepchars_in, stoplist_in, mode_in);
}

var operator() (in instr) {
 var sepchars_in;
 var stoplist_in;
 var mode_in;
 return operator()(instr, sepchars_in, stoplist_in, mode_in);
}

var operator() (in instr, in sepchars) {
 var stoplist_in;
 var mode_in;
 return operator()(instr, sepchars, stoplist_in, mode_in);
}

var operator() (in instr, in sepchars, in stoplist) {
 var mode_in;
 return operator()(instr, sepchars, stoplist, mode_in);
}

};
efb_xref xref{mv};
//#endif
