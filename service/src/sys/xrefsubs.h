//generated by exodus "compile xrefsubs"
//#ifndef EXODUSDLFUNC_XREFSUBS_H
#define EXODUSDLFUNC_XREFSUBS_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_xrefsubs;
class efb_xrefsubs : public ExodusFunctorBase
{
public:

efb_xrefsubs(MvEnvironment& mv) : ExodusFunctorBase("xrefsubs", "exodusprogrambasecreatedelete_", mv) {}

efb_xrefsubs& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, in subfn, in masterfilename, io masterfile, in masterfn, io locklist)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,io,in,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_xrefsubs.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_xrefsubs.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,subfn,masterfilename,masterfile,masterfn,locklist);

}

var operator() () {
 var mode_in;
 var subfn_in;
 var masterfilename_in;
 var masterfile_io;
 var masterfn_in;
 var locklist_io;
 return operator()(mode_in, subfn_in, masterfilename_in, masterfile_io, masterfn_in, locklist_io);
}

var operator() (in mode) {
 var subfn_in;
 var masterfilename_in;
 var masterfile_io;
 var masterfn_in;
 var locklist_io;
 return operator()(mode, subfn_in, masterfilename_in, masterfile_io, masterfn_in, locklist_io);
}

var operator() (in mode, in subfn) {
 var masterfilename_in;
 var masterfile_io;
 var masterfn_in;
 var locklist_io;
 return operator()(mode, subfn, masterfilename_in, masterfile_io, masterfn_in, locklist_io);
}

var operator() (in mode, in subfn, in masterfilename) {
 var masterfile_io;
 var masterfn_in;
 var locklist_io;
 return operator()(mode, subfn, masterfilename, masterfile_io, masterfn_in, locklist_io);
}

var operator() (in mode, in subfn, in masterfilename, io masterfile) {
 var masterfn_in;
 var locklist_io;
 return operator()(mode, subfn, masterfilename, masterfile, masterfn_in, locklist_io);
}

var operator() (in mode, in subfn, in masterfilename, io masterfile, in masterfn) {
 var locklist_io;
 return operator()(mode, subfn, masterfilename, masterfile, masterfn, locklist_io);
}

};
efb_xrefsubs xrefsubs{mv};
//#endif