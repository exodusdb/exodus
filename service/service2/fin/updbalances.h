//generated by exodus "compile ../fin/updbalances"
//#ifndef EXODUSDLFUNC_UPDBALANCES_H
#define EXODUSDLFUNC_UPDBALANCES_H

//a member variable/object to cache a pointer/object for the shared library function
//ExodusFunctorBase efb_updbalances;
class efb_updbalances : private ExodusFunctorBase
{
public:

efb_updbalances(MvEnvironment& mv) : ExodusFunctorBase("updbalances", "exodusprogrambasecreatedelete_", mv) {}

efb_updbalances& operator=(const var& newlibraryname) {
        closelib();
        libraryname_=newlibraryname.toString();
        return (*this);
}

//a member function with the right arguments, returning a var or void
var operator() (in balancesfile, in balanceskey, in deleting, in balanceperiod, in amount, in baseamount, in xxxxxx, in basecurrency, in vouchertype)
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
 typedef var (ExodusProgramBase::*pExodusProgramBaseMemberFunction)(in,in,in,in,in,in,in,in,in);

 //call the shared library object main function with the right args,
 // returning a var or void
 //return CALLMEMBERFUNCTION(*(efb_updbalances.pobject_),
 //((pExodusProgramBaseMemberFunction) (efb_updbalances.pmemberfunction_)))
 // (mode);
 return CALLMEMBERFUNCTION(*(this->pobject_),
 ((pExodusProgramBaseMemberFunction) (this->pmemberfunction_)))
  (balancesfile,balanceskey,deleting,balanceperiod,amount,baseamount,xxxxxx,basecurrency,vouchertype);

}

};
efb_updbalances updbalances{mv};
//#endif
