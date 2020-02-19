#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=AD.c BOARD.c ES_Framework.c IO_Ports.c RoachFrameworkEvents.c RoachHSM.c Stack.c SubHSM_GetCrown.c SubHSM_GetInside.c SubHSM_GetOutMoat.c SubHSM_GoBeacon1.c SubHSM_GoBeacon2.c SubHSM_GoBeacon3.c SubHSM_LocateBackWall.c SubHSM_Locate_Beacon.c SubHSM_ReturnCrown.c pwm.c roach.c serial.c RoachMain.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/AD.o ${OBJECTDIR}/BOARD.o ${OBJECTDIR}/ES_Framework.o ${OBJECTDIR}/IO_Ports.o ${OBJECTDIR}/RoachFrameworkEvents.o ${OBJECTDIR}/RoachHSM.o ${OBJECTDIR}/Stack.o ${OBJECTDIR}/SubHSM_GetCrown.o ${OBJECTDIR}/SubHSM_GetInside.o ${OBJECTDIR}/SubHSM_GetOutMoat.o ${OBJECTDIR}/SubHSM_GoBeacon1.o ${OBJECTDIR}/SubHSM_GoBeacon2.o ${OBJECTDIR}/SubHSM_GoBeacon3.o ${OBJECTDIR}/SubHSM_LocateBackWall.o ${OBJECTDIR}/SubHSM_Locate_Beacon.o ${OBJECTDIR}/SubHSM_ReturnCrown.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/roach.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/RoachMain.o
POSSIBLE_DEPFILES=${OBJECTDIR}/AD.o.d ${OBJECTDIR}/BOARD.o.d ${OBJECTDIR}/ES_Framework.o.d ${OBJECTDIR}/IO_Ports.o.d ${OBJECTDIR}/RoachFrameworkEvents.o.d ${OBJECTDIR}/RoachHSM.o.d ${OBJECTDIR}/Stack.o.d ${OBJECTDIR}/SubHSM_GetCrown.o.d ${OBJECTDIR}/SubHSM_GetInside.o.d ${OBJECTDIR}/SubHSM_GetOutMoat.o.d ${OBJECTDIR}/SubHSM_GoBeacon1.o.d ${OBJECTDIR}/SubHSM_GoBeacon2.o.d ${OBJECTDIR}/SubHSM_GoBeacon3.o.d ${OBJECTDIR}/SubHSM_LocateBackWall.o.d ${OBJECTDIR}/SubHSM_Locate_Beacon.o.d ${OBJECTDIR}/SubHSM_ReturnCrown.o.d ${OBJECTDIR}/pwm.o.d ${OBJECTDIR}/roach.o.d ${OBJECTDIR}/serial.o.d ${OBJECTDIR}/RoachMain.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/AD.o ${OBJECTDIR}/BOARD.o ${OBJECTDIR}/ES_Framework.o ${OBJECTDIR}/IO_Ports.o ${OBJECTDIR}/RoachFrameworkEvents.o ${OBJECTDIR}/RoachHSM.o ${OBJECTDIR}/Stack.o ${OBJECTDIR}/SubHSM_GetCrown.o ${OBJECTDIR}/SubHSM_GetInside.o ${OBJECTDIR}/SubHSM_GetOutMoat.o ${OBJECTDIR}/SubHSM_GoBeacon1.o ${OBJECTDIR}/SubHSM_GoBeacon2.o ${OBJECTDIR}/SubHSM_GoBeacon3.o ${OBJECTDIR}/SubHSM_LocateBackWall.o ${OBJECTDIR}/SubHSM_Locate_Beacon.o ${OBJECTDIR}/SubHSM_ReturnCrown.o ${OBJECTDIR}/pwm.o ${OBJECTDIR}/roach.o ${OBJECTDIR}/serial.o ${OBJECTDIR}/RoachMain.o

# Source Files
SOURCEFILES=AD.c BOARD.c ES_Framework.c IO_Ports.c RoachFrameworkEvents.c RoachHSM.c Stack.c SubHSM_GetCrown.c SubHSM_GetInside.c SubHSM_GetOutMoat.c SubHSM_GoBeacon1.c SubHSM_GoBeacon2.c SubHSM_GoBeacon3.c SubHSM_LocateBackWall.c SubHSM_Locate_Beacon.c SubHSM_ReturnCrown.c pwm.c roach.c serial.c RoachMain.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/AD.o: AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AD.o.d 
	@${RM} ${OBJECTDIR}/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/AD.o.d" -o ${OBJECTDIR}/AD.o AD.c   
	
${OBJECTDIR}/BOARD.o: BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BOARD.o.d 
	@${RM} ${OBJECTDIR}/BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/BOARD.o.d" -o ${OBJECTDIR}/BOARD.o BOARD.c   
	
${OBJECTDIR}/ES_Framework.o: ES_Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/ES_Framework.o 
	@${FIXDEPS} "${OBJECTDIR}/ES_Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ES_Framework.o.d" -o ${OBJECTDIR}/ES_Framework.o ES_Framework.c   
	
${OBJECTDIR}/IO_Ports.o: IO_Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/IO_Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/IO_Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/IO_Ports.o.d" -o ${OBJECTDIR}/IO_Ports.o IO_Ports.c   
	
${OBJECTDIR}/RoachFrameworkEvents.o: RoachFrameworkEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RoachFrameworkEvents.o.d 
	@${RM} ${OBJECTDIR}/RoachFrameworkEvents.o 
	@${FIXDEPS} "${OBJECTDIR}/RoachFrameworkEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RoachFrameworkEvents.o.d" -o ${OBJECTDIR}/RoachFrameworkEvents.o RoachFrameworkEvents.c   
	
${OBJECTDIR}/RoachHSM.o: RoachHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RoachHSM.o.d 
	@${RM} ${OBJECTDIR}/RoachHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/RoachHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RoachHSM.o.d" -o ${OBJECTDIR}/RoachHSM.o RoachHSM.c   
	
${OBJECTDIR}/Stack.o: Stack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Stack.o.d 
	@${RM} ${OBJECTDIR}/Stack.o 
	@${FIXDEPS} "${OBJECTDIR}/Stack.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Stack.o.d" -o ${OBJECTDIR}/Stack.o Stack.c   
	
${OBJECTDIR}/SubHSM_GetCrown.o: SubHSM_GetCrown.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GetCrown.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GetCrown.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GetCrown.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GetCrown.o.d" -o ${OBJECTDIR}/SubHSM_GetCrown.o SubHSM_GetCrown.c   
	
${OBJECTDIR}/SubHSM_GetInside.o: SubHSM_GetInside.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GetInside.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GetInside.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GetInside.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GetInside.o.d" -o ${OBJECTDIR}/SubHSM_GetInside.o SubHSM_GetInside.c   
	
${OBJECTDIR}/SubHSM_GetOutMoat.o: SubHSM_GetOutMoat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GetOutMoat.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GetOutMoat.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GetOutMoat.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GetOutMoat.o.d" -o ${OBJECTDIR}/SubHSM_GetOutMoat.o SubHSM_GetOutMoat.c   
	
${OBJECTDIR}/SubHSM_GoBeacon1.o: SubHSM_GoBeacon1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon1.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon1.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GoBeacon1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GoBeacon1.o.d" -o ${OBJECTDIR}/SubHSM_GoBeacon1.o SubHSM_GoBeacon1.c   
	
${OBJECTDIR}/SubHSM_GoBeacon2.o: SubHSM_GoBeacon2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon2.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon2.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GoBeacon2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GoBeacon2.o.d" -o ${OBJECTDIR}/SubHSM_GoBeacon2.o SubHSM_GoBeacon2.c   
	
${OBJECTDIR}/SubHSM_GoBeacon3.o: SubHSM_GoBeacon3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon3.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon3.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GoBeacon3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GoBeacon3.o.d" -o ${OBJECTDIR}/SubHSM_GoBeacon3.o SubHSM_GoBeacon3.c   
	
${OBJECTDIR}/SubHSM_LocateBackWall.o: SubHSM_LocateBackWall.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_LocateBackWall.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_LocateBackWall.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_LocateBackWall.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_LocateBackWall.o.d" -o ${OBJECTDIR}/SubHSM_LocateBackWall.o SubHSM_LocateBackWall.c   
	
${OBJECTDIR}/SubHSM_Locate_Beacon.o: SubHSM_Locate_Beacon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_Locate_Beacon.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_Locate_Beacon.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_Locate_Beacon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_Locate_Beacon.o.d" -o ${OBJECTDIR}/SubHSM_Locate_Beacon.o SubHSM_Locate_Beacon.c   
	
${OBJECTDIR}/SubHSM_ReturnCrown.o: SubHSM_ReturnCrown.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_ReturnCrown.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_ReturnCrown.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_ReturnCrown.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_ReturnCrown.o.d" -o ${OBJECTDIR}/SubHSM_ReturnCrown.o SubHSM_ReturnCrown.c   
	
${OBJECTDIR}/pwm.o: pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/pwm.o.d" -o ${OBJECTDIR}/pwm.o pwm.c   
	
${OBJECTDIR}/roach.o: roach.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/roach.o.d 
	@${RM} ${OBJECTDIR}/roach.o 
	@${FIXDEPS} "${OBJECTDIR}/roach.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/roach.o.d" -o ${OBJECTDIR}/roach.o roach.c   
	
${OBJECTDIR}/serial.o: serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serial.o.d 
	@${RM} ${OBJECTDIR}/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/serial.o.d" -o ${OBJECTDIR}/serial.o serial.c   
	
${OBJECTDIR}/RoachMain.o: RoachMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RoachMain.o.d 
	@${RM} ${OBJECTDIR}/RoachMain.o 
	@${FIXDEPS} "${OBJECTDIR}/RoachMain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RoachMain.o.d" -o ${OBJECTDIR}/RoachMain.o RoachMain.c   
	
else
${OBJECTDIR}/AD.o: AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/AD.o.d 
	@${RM} ${OBJECTDIR}/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/AD.o.d" -o ${OBJECTDIR}/AD.o AD.c   
	
${OBJECTDIR}/BOARD.o: BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/BOARD.o.d 
	@${RM} ${OBJECTDIR}/BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/BOARD.o.d" -o ${OBJECTDIR}/BOARD.o BOARD.c   
	
${OBJECTDIR}/ES_Framework.o: ES_Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/ES_Framework.o 
	@${FIXDEPS} "${OBJECTDIR}/ES_Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ES_Framework.o.d" -o ${OBJECTDIR}/ES_Framework.o ES_Framework.c   
	
${OBJECTDIR}/IO_Ports.o: IO_Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/IO_Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/IO_Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/IO_Ports.o.d" -o ${OBJECTDIR}/IO_Ports.o IO_Ports.c   
	
${OBJECTDIR}/RoachFrameworkEvents.o: RoachFrameworkEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RoachFrameworkEvents.o.d 
	@${RM} ${OBJECTDIR}/RoachFrameworkEvents.o 
	@${FIXDEPS} "${OBJECTDIR}/RoachFrameworkEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RoachFrameworkEvents.o.d" -o ${OBJECTDIR}/RoachFrameworkEvents.o RoachFrameworkEvents.c   
	
${OBJECTDIR}/RoachHSM.o: RoachHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RoachHSM.o.d 
	@${RM} ${OBJECTDIR}/RoachHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/RoachHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RoachHSM.o.d" -o ${OBJECTDIR}/RoachHSM.o RoachHSM.c   
	
${OBJECTDIR}/Stack.o: Stack.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Stack.o.d 
	@${RM} ${OBJECTDIR}/Stack.o 
	@${FIXDEPS} "${OBJECTDIR}/Stack.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Stack.o.d" -o ${OBJECTDIR}/Stack.o Stack.c   
	
${OBJECTDIR}/SubHSM_GetCrown.o: SubHSM_GetCrown.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GetCrown.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GetCrown.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GetCrown.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GetCrown.o.d" -o ${OBJECTDIR}/SubHSM_GetCrown.o SubHSM_GetCrown.c   
	
${OBJECTDIR}/SubHSM_GetInside.o: SubHSM_GetInside.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GetInside.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GetInside.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GetInside.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GetInside.o.d" -o ${OBJECTDIR}/SubHSM_GetInside.o SubHSM_GetInside.c   
	
${OBJECTDIR}/SubHSM_GetOutMoat.o: SubHSM_GetOutMoat.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GetOutMoat.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GetOutMoat.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GetOutMoat.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GetOutMoat.o.d" -o ${OBJECTDIR}/SubHSM_GetOutMoat.o SubHSM_GetOutMoat.c   
	
${OBJECTDIR}/SubHSM_GoBeacon1.o: SubHSM_GoBeacon1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon1.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon1.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GoBeacon1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GoBeacon1.o.d" -o ${OBJECTDIR}/SubHSM_GoBeacon1.o SubHSM_GoBeacon1.c   
	
${OBJECTDIR}/SubHSM_GoBeacon2.o: SubHSM_GoBeacon2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon2.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon2.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GoBeacon2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GoBeacon2.o.d" -o ${OBJECTDIR}/SubHSM_GoBeacon2.o SubHSM_GoBeacon2.c   
	
${OBJECTDIR}/SubHSM_GoBeacon3.o: SubHSM_GoBeacon3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon3.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_GoBeacon3.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_GoBeacon3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_GoBeacon3.o.d" -o ${OBJECTDIR}/SubHSM_GoBeacon3.o SubHSM_GoBeacon3.c   
	
${OBJECTDIR}/SubHSM_LocateBackWall.o: SubHSM_LocateBackWall.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_LocateBackWall.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_LocateBackWall.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_LocateBackWall.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_LocateBackWall.o.d" -o ${OBJECTDIR}/SubHSM_LocateBackWall.o SubHSM_LocateBackWall.c   
	
${OBJECTDIR}/SubHSM_Locate_Beacon.o: SubHSM_Locate_Beacon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_Locate_Beacon.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_Locate_Beacon.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_Locate_Beacon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_Locate_Beacon.o.d" -o ${OBJECTDIR}/SubHSM_Locate_Beacon.o SubHSM_Locate_Beacon.c   
	
${OBJECTDIR}/SubHSM_ReturnCrown.o: SubHSM_ReturnCrown.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SubHSM_ReturnCrown.o.d 
	@${RM} ${OBJECTDIR}/SubHSM_ReturnCrown.o 
	@${FIXDEPS} "${OBJECTDIR}/SubHSM_ReturnCrown.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SubHSM_ReturnCrown.o.d" -o ${OBJECTDIR}/SubHSM_ReturnCrown.o SubHSM_ReturnCrown.c   
	
${OBJECTDIR}/pwm.o: pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pwm.o.d 
	@${RM} ${OBJECTDIR}/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/pwm.o.d" -o ${OBJECTDIR}/pwm.o pwm.c   
	
${OBJECTDIR}/roach.o: roach.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/roach.o.d 
	@${RM} ${OBJECTDIR}/roach.o 
	@${FIXDEPS} "${OBJECTDIR}/roach.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/roach.o.d" -o ${OBJECTDIR}/roach.o roach.c   
	
${OBJECTDIR}/serial.o: serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/serial.o.d 
	@${RM} ${OBJECTDIR}/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/serial.o.d" -o ${OBJECTDIR}/serial.o serial.c   
	
${OBJECTDIR}/RoachMain.o: RoachMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/RoachMain.o.d 
	@${RM} ${OBJECTDIR}/RoachMain.o 
	@${FIXDEPS} "${OBJECTDIR}/RoachMain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RoachMain.o.d" -o ${OBJECTDIR}/RoachMain.o RoachMain.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/final_project_v5.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
