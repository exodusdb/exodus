//generated by exodus "compile updsecindex"
//#ifndef EXODUSDLFUNC_UPDSECINDEX_H
#define EXODUSDLFUNC_UPDSECINDEX_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_updsecindex;
class efb_updsecindex : public ExodusFunctorBase
{
public:

efb_updsecindex(MvEnvironment& mv) : ExodusFunctorBase("updsecindex", "exodusprogrambasecreatedelete_", mv) {}

efb_updsecindex& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in mode, in filename, io keys, in fieldnames, in oldvalues, in newvalues, io valid, io msg)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,io,in,in,in,io,io);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_updsecindex.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_updsecindex.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (mode,filename,keys,fieldnames,oldvalues,newvalues,valid,msg);

}

var operator() () {
 var mode_in;
 var filename_in;
 var keys_io;
 var fieldnames_in;
 var oldvalues_in;
 var newvalues_in;
 var valid_io;
 var msg_io;
 return operator()(mode_in, filename_in, keys_io, fieldnames_in, oldvalues_in, newvalues_in, valid_io, msg_io);
}

var operator() (in mode) {
 var filename_in;
 var keys_io;
 var fieldnames_in;
 var oldvalues_in;
 var newvalues_in;
 var valid_io;
 var msg_io;
 return operator()(mode, filename_in, keys_io, fieldnames_in, oldvalues_in, newvalues_in, valid_io, msg_io);
}

var operator() (in mode, in filename) {
 var keys_io;
 var fieldnames_in;
 var oldvalues_in;
 var newvalues_in;
 var valid_io;
 var msg_io;
 return operator()(mode, filename, keys_io, fieldnames_in, oldvalues_in, newvalues_in, valid_io, msg_io);
}

var operator() (in mode, in filename, io keys) {
 var fieldnames_in;
 var oldvalues_in;
 var newvalues_in;
 var valid_io;
 var msg_io;
 return operator()(mode, filename, keys, fieldnames_in, oldvalues_in, newvalues_in, valid_io, msg_io);
}

var operator() (in mode, in filename, io keys, in fieldnames) {
 var oldvalues_in;
 var newvalues_in;
 var valid_io;
 var msg_io;
 return operator()(mode, filename, keys, fieldnames, oldvalues_in, newvalues_in, valid_io, msg_io);
}

var operator() (in mode, in filename, io keys, in fieldnames, in oldvalues) {
 var newvalues_in;
 var valid_io;
 var msg_io;
 return operator()(mode, filename, keys, fieldnames, oldvalues, newvalues_in, valid_io, msg_io);
}

var operator() (in mode, in filename, io keys, in fieldnames, in oldvalues, in newvalues) {
 var valid_io;
 var msg_io;
 return operator()(mode, filename, keys, fieldnames, oldvalues, newvalues, valid_io, msg_io);
}

var operator() (in mode, in filename, io keys, in fieldnames, in oldvalues, in newvalues, io valid) {
 var msg_io;
 return operator()(mode, filename, keys, fieldnames, oldvalues, newvalues, valid, msg_io);
}

};
efb_updsecindex updsecindex{mv};
//#endif