#include "stdafx.h"
#include "vsm.hpp"

#define VSMModel_key 0x00000000

class vsmDsim : public IDSIMMODEL
{
public:
	virtual INT isdigital(CHAR* pinname) override { return 1; }
	virtual VOID setup(IINSTANCE* inst, IDSIMCKT* dsim) override { this->inst = inst; this->ckt = dsim; }
	virtual VOID runctrl(RUNMODES mode) override {}
	virtual VOID actuate(REALTIME time, ACTIVESTATE newstate) override {}
	virtual BOOL indicate(REALTIME time, ACTIVEDATA* data) override { return TRUE; }
	virtual VOID simulate(ABSTIME time, DSIMMODES mode) override {}
	virtual VOID callback(ABSTIME time, EVENTID eventid) override {}
private:
	IINSTANCE* inst = NULL;
	IDSIMCKT* ckt = NULL;
};

class vsmActive : public IACTIVEMODEL {
public:
    virtual VOID initialize(ICOMPONENT* cpt) override { this->cpt = cpt; }
    virtual ISPICEMODEL* getspicemodel(CHAR* p) override { return NULL; }
    virtual IDSIMMODEL* getdsimmodel(CHAR* p) override { return new vsmDsim; }
    virtual VOID plot(ACTIVESTATE state) override {}
    virtual VOID animate(INT element, ACTIVEDATA* newstate) override {}
    virtual BOOL actuate(WORD key, INT x, INT y, DWORD flags) override { return FALSE; }
private:
	ICOMPONENT* cpt = NULL;
};

extern "C" {
	IDSIMMODEL __declspec (dllexport)* createdsimmodel(CHAR* device, ILICENCESERVER* ils) {
		return new vsmDsim;
	}
}

extern "C" VOID __declspec (dllexport) deletedsimmodel(IDSIMMODEL* model) {
	delete (IDSIMMODEL*)model;
}

extern "C" {
	__declspec(dllexport) IACTIVEMODEL* createactivemodel(CHAR* device, ILICENCESERVER* ils) {
		return new vsmActive;
	}
}

extern "C" {
	VOID  __declspec(dllexport) deleteactivemodel(IACTIVEMODEL* model) {
		delete (IACTIVEMODEL*)model;
	}
}
