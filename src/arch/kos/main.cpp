/*******************************************************************************
 * Copyright (c) 2006 - 2018 ACIN, Profactor GmbH, AIT, fortiss GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *  Alois Zoitl, Gerhard Ebenhofer, Thomas Strasser, Rene Smodic,
 *  Martin Melik Merkumians, Ingo Hegny, Filip Andren - initial API and implementation and/or initial documentation
 *  Tarik Terzimehic - make OPC UA server port setable from the command line
 *******************************************************************************/
#include <fortenew.h>
#include <stdio.h>
#include <signal.h>
#include "../../stdfblib/ita/RMT_DEV.h"

#include "../utils/mainparam_utils.h"

#include <platform/platform.h>
#include <kos_net.h>

#include <iostream>
#include <fstream>
#include <string>



#ifdef FORTE_ROS
#include <ros/ros.h>
#endif //FORTE_ROS

#ifdef CONFIG_POWERLINK_USERSTACK
#include <EplWrapper.h>
#endif

/*!\brief Check if the correct endianess has been configured.
 *
 * If the right endianess is not set this function will end FORTE.
 */
void checkEndianess();

//this keeps away a lot of rtti and exception handling stuff
extern "C" void __cxa_pure_virtual(void){
  //TODO maybe add some reporting here
  //Although we should never get here
  //if we are here something very very bad has happened e.g., stack overflow or other memory corruption

}

RMT_DEV *poDev = 0;

void endForte(int pa_nSig){
  (void) pa_nSig;
  if(0 != poDev){
    poDev->changeFBExecutionState(cg_nMGM_CMD_Kill);
  }
}

/*!\brief Creates the Device-Object
 * \param pa_acMGRID A string containing IP and Port like [IP]:[Port]
 */
void createDev(const char *pa_acMGRID){

  signal(SIGINT, endForte);
  signal(SIGTERM, endForte);
  signal(SIGHUP, endForte);

#ifdef CONFIG_POWERLINK_USERSTACK
  CEplStackWrapper::eplMainInit();
#endif

  poDev = new RMT_DEV;

  poDev->setMGR_ID(pa_acMGRID);
  poDev->startDevice();
  DEVLOG_INFO("FORTE is up and running\n");
  poDev->MGR.joinResourceThread();
  DEVLOG_INFO("FORTE finished\n");
  delete poDev;
}


void FileReadTest()
{
    std::ifstream userListFile;
    std::string userListFileName = "/c/forte.fboot";

    userListFile.open(userListFileName);
    if (!userListFile.is_open())
    {
       DEVLOG_INFO("FORTE fail read fboot \n");
       return;
    }

    DEVLOG_INFO("FORTE read fboot - OK \n");

    userListFile.close();
}



int main(int argc, char *arg[]){
  
  DEVLOG_INFO("FORTE try start....\n");

  FileReadTest();

/*#define server_addr "10.0.2.2"
    // Add network interface. 
    if (!configure_net_iface(DEFAULT_INTERFACE, "10.0.2.10", DEFAULT_MASK, DEFAULT_GATEWAY, DEFAULT_MTU)) {

        perror(DEFAULT_INTERFACE ": network iface configuration failed\n");
        return EXIT_FAILURE;
    }

    if (!list_network_ifaces()) {
        perror("listing of host network interfaces failes\n");
        return EXIT_FAILURE;
    }*/




  checkEndianess();

#ifdef FORTE_ROS
  if(argc <= 1){ //! Default Value (localhost:61499)
    std::string rosdistro = "indigo";
    if (rosdistro == (std::string)std::getenv("ROS_DISTRO")){
      DEVLOG_INFO("path to forte.exe: %s \n", arg[0]);
      ros::init(argc, arg, "ros_Functionblocks_in_FORTE");
    }
  }
#endif //FORTE_ROS

  char kosArgv[argc+1][200];
  int i =0;
  for (int i=0;i<argc;i++) {
    strcpy (kosArgv[i],arg[i]);
  }
  strcpy (kosArgv[i++],"-f");
  strcpy (kosArgv[i],"//data//forte.fboot");

//  const char *pIpPort = parseCommandLineArguments(argc, arg);

  const char *pIpPort = parseCommandLineArguments(i, (char**)kosArgv);



  if((0 != strlen(pIpPort)) && (NULL != strchr(pIpPort, ':'))){
    createDev(pIpPort);
  }
  else{ //! Lists the help for FORTE
    listHelp();
  }

  return 0;
}

void checkEndianess(){
  int i = 1;
  char *p = (char *) &i;
  if(p[0] == 1){
    //we are on a little endian platform
#ifdef FORTE_BIG_ENDIAN
    DEVLOG_ERROR("Wrong endianess configured! You are on a little endian platform and have configured big endian!\n");
    exit(-1);
#endif
  }
  else{
    //we are on a big endian platform
#ifdef FORTE_LITTLE_ENDIAN
    DEVLOG_ERROR("Wrong endianess configured! You are on a big endian platform and have configured little endian!\n");
    exit(-1);
#endif
  }
}
