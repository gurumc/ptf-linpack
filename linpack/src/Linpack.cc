/**
   @file    Linpack.cc
   @ingroup Linpack
   @brief   Skeleton of a Plugin
   @author  Author's name
   @verbatim
    Revision:       $Revision$
    Revision date:  $Date$
    Committed by:   $Author$

    This file is part of the Periscope performance measurement tool.
    See http://www.lrr.in.tum.de/periscope for details.

    Copyright (c) 2005-2014, Technische Universitaet Muenchen, Germany
    See the COPYING file in the base directory of the package for details.
   @endverbatim
 */

#include "Linpack.h"

static int hplcounter=1;
/**
 * @brief Initialize the plugin's data structures.
 *
 * The tuning parameter list needs to be created. The tuning parameters can be obtained
 * from the SIR file, by calling the extractTuningParameter function as follows:
 *
 *    tuningParameters = extractTuningParameters(sirFilePath);
 *
 * Search algorithms are loaded here when required. This can be done as follows:
 *
 *    searchAlgorithm = loadSearchAlgorithm("name");
 *
 * where "name" refers to one of the available search algorithms (currently only exhaustive).
 *
 * @ingroup Linpack
 *
 * @param context a pointer to a context for a plugin
 * @param pool_set a pointer to a set of pools for a plugin
 */
void Linpack::initialize(DriverContext *context, ScenarioPoolSet *pool_set) {
	printf("**********Linpack::initialize****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to initialize()\n");

	this->context = context;
	this->pool_set = pool_set;

	/**
	 * Problem Size ranging from 6000 to 10000 [Increasing in steps of 1000]
	 */
	TuningParameter *inputSizeTP = new TuningParameter();
	inputSizeTP->setId(0);
	inputSizeTP->setName("PROBLEMSIZE");
	inputSizeTP->setRange(6000, 10000, 1000); // (start, max, step)
	tuningParameters.push_back(inputSizeTP);

	/**
	 * Block size ranging from 64 to 256 in steps of 16
	 */
	TuningParameter *blockSizeTP = new TuningParameter();
	blockSizeTP->setId(1);
	blockSizeTP->setName("BLOCKSIZE");
	blockSizeTP->setRange(128, 256, 16); // (start, max, step)
	tuningParameters.push_back(blockSizeTP);

	/**
	 * Process Grid P - Number of process grids
	 */
	TuningParameter *procGrid_PTP = new TuningParameter();
	procGrid_PTP->setId(2);
	procGrid_PTP->setName("PROC_GRID_P");
	procGrid_PTP->setRange(2, 2, 1); // (start, max, step)
	tuningParameters.push_back(procGrid_PTP);

	TuningParameter *procGrid_QTP = new TuningParameter();
	procGrid_QTP->setId(3);
	procGrid_QTP->setName("PROC_GRID_Q");
	procGrid_QTP->setRange(2, 4, 1); // (start, max, step)
	tuningParameters.push_back(procGrid_QTP);

	/**
	 * Process mapping(0- Row , 1 -Column major)
	 */
	TuningParameter *pMappingTP = new TuningParameter();
	pMappingTP->setId(4);
	pMappingTP->setName("Process_Mapping");
	pMappingTP->setRange(0, 1, 1); // (start, max, step)
	tuningParameters.push_back(pMappingTP);

//	/***
//	 * Broadcast
//	 */
//	TuningParameter *bcastsTP = new TuningParameter();
//	bcastsTP->setId(5);
//	bcastsTP->setName("BCAST");
//	bcastsTP->setRange(3, 5, 1); // (start, max, step)
//	tuningParameters.push_back(bcastsTP);


//	string results = pMappingTP->toString();
//	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack plugin: Before exiting to initialize, Tuning parameter is: %s\n\n",
//	  results.c_str());

	int major, minor;
	string name, description;

	//Load the search algorithm which we want to use
	context->loadSearchAlgorithm("exhaustive", &major, &minor, &name, &description);
	searchAlgorithm = context->getSearchAlgorithmInstance("exhaustive");
	if (searchAlgorithm){
	print_loaded_search(major, minor, name, description);
	//Initialize the search algorithm with content and pool_set values from runtime system
	searchAlgorithm->initialize(context, pool_set);
	}
}

/**
 * @brief Operations to be done at the start of a tuning step.
 *
 * The tuning parameters that were created at initialization are used to create a variant space.
 * The variant space and the regions are then used to create a search space that is passed to the
 * search algorithm.
 *
 * @ingroup Linpack
 *
 */
void Linpack::startTuningStep(void) {
	printf("**********Linpack::startTuningStep****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to startTuningStep()\n");

	VariantSpace *variantSpace=new VariantSpace();
	SearchSpace *searchSpace=new SearchSpace();

	for (int i = 0; i < tuningParameters.size(); i++) {
	  variantSpace->addTuningParameter(tuningParameters[i]);
	}
	searchSpace->setVariantSpace(variantSpace);
	searchSpace->addRegion(appl->get_phase_region());//Madhu added this line

	searchAlgorithm->addSearchSpace(searchSpace);
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: Created a SearchSpace for Linpack.\n");

}

/**
 * @brief Defines a pre-analysis strategy.
 *
 * Found properties of the pre-analysis strategy are stored in the arp.
 *
 * @ingroup Linpack
 *
 * @return true if pre-analysis is required false otherwise
 *
 */
bool Linpack::analysisRequired(StrategyRequest** strategy) {
	printf("**********Linpack::analysisRequired****************\n");
	return false;
}

/**
 * @brief The Created Scenario Pool (csp) is populated here.
 *
 * The scenarios need to be created and added to the first pool. To create the scenarios, a
 * search algorithm can be used or it can be done directly by the plugin.
 *
 * After this step, the Periscope will verify that scenarios were added to the csp.
 *
 * @ingroup Linpack
 *
 */
void Linpack::createScenarios(void) {
	printf("**********Linpack::createScenarios start****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to createScenarios()\n");
	searchAlgorithm->createScenarios();
	printf("**********Linpack::createScenarios end****************\n");
}

/**
 * @brief Preparatory steps for the scenarios are done here.
 *
 * If there are any preparatory steps required by some or all scenarios in the csp (for example:
 * the project may need to be re-compiled), then they are to be performed here. After each
 * scenario is prepared, they are migrated from the csp to the Prepared Scenario Pool (psp).
 *
 * In some cases, no preparation may be necessary and the plugin can simply move all scenarios
 * from the csp to the psp.
 *
 * After this step, the Periscope will verify that scenarios were added to the psp.
 *
 * @ingroup Linpack
 *
 */
void Linpack::prepareScenarios(void) {
	printf("**********Linpack::prepareScenarios****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to prepareScenarios()\n");

	if (!pool_set->csp->empty()) {
		Scenario* scenario;
		scenario = pool_set->csp->pop();

		//Get integral value of Variant and using this change the HPL.dat file
		//iterate the list of tuning param and update it to HPL.dat

		//gmc: For iterating write a loop and get the values and write it

		const Variant* v = scenario->getTuningSpecifications()->front()->getVariant();
		map<TuningParameter*,int> tpValues = v->getValue();
		long problemSize = tpValues[tuningParameters[0]];
		long blockSize = tpValues[tuningParameters[1]];
		long pMapping = tpValues[tuningParameters[2]];
		long qMapping = tpValues[tuningParameters[3]];
		long procMapping = tpValues[tuningParameters[4]];

		char cmnd[1000];
		int retVal;

		sprintf(cmnd,"sed -e '6s/.*/%ld/' -e '8s/.*/%ld/' -e '11s/.*/%ld/' -e '12s/.*/%ld/' -e '9s/.*/%ld/' -e '3s/.*/HPL%d.out/' <HPL.dat.sed >HPL.dat ", problemSize, blockSize, pMapping, qMapping, procMapping, hplcounter++);
		retVal = system(cmnd);

		if(retVal) {
			psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: Error while modifying HPL.dat, Aborting.. \n");
			return;
		} else {
			psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: HPL.dat file modified successfully in prepareScenarios()\n");
		}

		if(hplcounter - 2) {
			sprintf(cmnd,"awk '/WR/{print %d \"\\t\"$6 \"\\t\"$7}' HPL%d.out >> TimeAndGflops", (hplcounter - 2),(hplcounter - 2));
			retVal = system(cmnd);

			if(retVal) {
				psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: Error while running awk command in prepareScenarios(), Aborting.. \n");
				return;
			} else {
				psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: awk ran successfully and modified file TimeAndGflops in prepareScenarios()\n");
			}
		}

		//TODO: Get all integral values and write it to HPL. Make it fancy if you have time
		//prepare command to modify HPL.dat file and call system call in it
		pool_set->psp->push(scenario);
	}
}


/**
 * @brief Populate the Experiment Scenario Pool (esp) for the next experiment.
 *
 * This is the final step before the experiments are executed. Scenarios are moved from the
 * psp to the esp, depending on the number of processes and whether they can be executed
 * in parallel.
 *
 * After this step, the Periscope will verify that scenarios were added to the esp.
 *
 * @ingroup Linpack
 *
 * @param numprocs         Number of processes required for the experiment
 * @param analysisRequired Defines is per-experiment analysis required
 * @param strategy         Strategy request for per-experiment analysis
 *
 */
void Linpack::defineExperiment(int numprocs, bool *analysisRequired, StrategyRequest** strategy) {
	printf("**********Linpack::defineExperiment****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to defineExperiment()\n");

	Scenario *scenario = pool_set->psp->pop();
	scenario->setSingleTunedRegionWithPropertyRank(NULL, EXECTIME, 0);
	pool_set->esp->push(scenario);
}

/**
 * @brief Returns does the application has to be restarted for the next experiment.
 *
 * Return true if a restart of the application is required for the next experiment,
 * false otherwise.
 *
 * @ingroup Linpack
 *
 * @return true if an application has to be restarted false otherwise
 *
 */
bool Linpack::restartRequired(string *env, int *numprocs, string *command, bool *is_instrumented) {
	printf("**********Linpack::restartRequired****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to restartRequired()\n");

	*is_instrumented = false;
	return true;
}

/**
 * @brief Returns the status of the current search iteration.
 *
 * Returns true if the current search iteration is finished (by either the plugin itself or as
 * a result of the search algorithm), false otherwise.
 *
 * @ingroup Linpack
 *
 * @return true if the plugin has finished search false otherwise
 *
 */
bool Linpack::searchFinished(void) {
	printf("**********Linpack::searchFinished****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to searchFinished()\n");
	return searchAlgorithm->searchFinished();
}

/**
 * @brief Final operation of a tuning step.
 *
 * If any post-processing is necessary before entering the next tuning iteration, it is to be 
 * done here.
 *
 * @ingroup Linpack
 *
 */
void Linpack::finishTuningStep(void) {
	printf("**********Linpack::finishTuningStep****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to processResults()\n");

}

/**
 * @brief Returns true if the plugin finished the tuning process, false otherwise.
 *
 * @ingroup Linpack
 *
 * @return true if the plugin has finished tuning false otherwise
 *
 */
bool Linpack::tuningFinished(void) {
	printf("**********Linpack::tuningFinished****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to tuningFinished()\n");

	return true;
}

void extract(void) {
	FILE *fp;
	double myvar;
	int i, j;
	double values[20][3];
	double max = 0;
	int line = 0;
	int lines = 0;
	char ch;

	fp = fopen("TimeAndGflops", "r");

	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == '\n') {
			lines++;
		}
	}
	rewind(fp);
	printf("lines = %d\n", lines);

	for (i = 0; i < lines; i++) {
		for (j = 0; j < 3; j++) {
			fscanf(fp, "%lf", &myvar);

			values[i][j] = myvar;
			if (j == 2 && values[i][j] > max) {
				max = values[i][j];
				line = i;
			}
		}
	}
	printf("max value = %.3f at line %d\n", max, line + 1);
	fclose(fp);

	fp = fopen("result.log","w+");

	fprintf(fp, "Maximum Gflops achieved = %.3f GFlops, Scenario id: %d\n", max, line + 1);

	fclose(fp);
}

/**
 * @brief Prints to the screen (and to a file, where necessary) the tuning advice.
 *
 * @ingroup Linpack
 */
Advice *Linpack::getAdvice(void) {
	printf("**********Linpack::getAdvice****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to getAdvice()\n");

	char cmnd[1000];
	int retVal;

	printf("***********hplcounter value***************\n"+ hplcounter);
	sprintf(cmnd,"awk '/WR/{print %d \"\\t\"$6 \"\\t\"$7}' HPL%d.out >> TimeAndGflops", (hplcounter - 1),(hplcounter - 1));
	retVal = system(cmnd);

	if(retVal) {
		psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: Error while running awk command in getAdvice(), Aborting.. \n");
	} else {
		psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: awk cmd executes successfully and updates the file TimeAndGflops in getAdvice()\n");
	}

	extract();


	sprintf(cmnd,"sort -gr -k3 TimeAndGflops | head -1 | awk '{print $3\" at line \"$1}' >> Result");
	retVal = system(cmnd);

	if(retVal) {
		psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: Error while running awk command in getAdvice(), Aborting.. \n");
	} else {
		psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: awk cmd executes successfully and updates the file TimeAndGflops in getAdvice()\n");
	}


	std::ostringstream result_oss;
	map<int,double> timeForScenario=searchAlgorithm->getSearchPath();
	double serialTime = timeForScenario[0];

	int optimum = searchAlgorithm->getOptimum();
	result_oss << "Optimum Scenario: " << optimum << endl << endl;

	cout << result_oss.str();

	//TODO: search and get the best value and display it to the user

	return new Advice();
}

/**
 * @brief Finalize the plugin normally.
 *
 * Remove any allocated memory, objects, file descriptors, etc.
 *
 * @ingroup Linpack
 *
 */
void Linpack::finalize() {
	printf("**********Linpack::finalize****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to finalize()\n");

	delete searchAlgorithm;
	for (int i = 0; i < tuningParameters.size(); i++) {
	  delete tuningParameters[i];
	}
}

/**
 * @brief Terminate the plugin due to error.
 *
 * Safely remove any allocated memory, objects, file descriptors, etc. This method should
 * be able to be executed safely at any point.
 *
 * @ingroup Linpack
 *
 */
void Linpack::terminate() {
	printf("**********Linpack::terminate****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to terminate()\n");
}

/*
 * The following are C functions that belong to the Plugin Management Interface, and not to the
 * IPlugin class. Currently the interface includes information and instance creation functions.
 *
 * These are defined in PluginManagement.h .
 */

/**
 * @brief Returns an instance of this particular plugin implementation.
 *
 * Typically, a simple return with new is enough. For example:
 *
 * return a pointer to a new Linpack();
 *
 * @ingroup Linpack
 *
 * @return A pointer to a new Linpack
 */
IPlugin* getPluginInstance(void) {
	printf("**********Linpack::getPluginInstance****************\n");
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to getComponentInstance()\n");

	return new Linpack();
}

/**
 * @brief Returns the major plugin interface version used by this plugin (example: the 1 in 1.0).
 *
 * @ingroup Linpack
 *
 * @return The major plugin interface version used by the plugin
 */
int getVersionMajor(void) {
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to getInterfaceVersionMajor()\n");

	return 1;
}

/**
 * @brief Returns the minor plugin interface version used by this plugin (example: the 0 in 1.0).
 *
 * @ingroup Linpack
 *
 * @return The minor plugin interface version used by the plugin
 *
 */
int getVersionMinor(void) {
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to getInterfaceVersionMinor()\n");

	return 0;
}

/**
 * @brief Returns a string with the name of the plugin.
 *
 * @ingroup Linpack
 *
 * @return Returns a string with the name of the plugin
 *
 */
string getName(void) {
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to getName()\n");

	return "Linpack";
}

/**
 * @brief Returns a string with a short description of the plugin.
 *
 * @ingroup Linpack
 *
 * @return A string with a short description of the plugin
 *
 */
string getShortSummary(void) {
	psc_dbgmsg(PSC_SELECTIVE_DEBUG_LEVEL(AutotunePlugins), "Linpack: call to getShortSummary()\n");

	return "Explores scalability of HPL.";
}
