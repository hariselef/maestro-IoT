# STM32 Project Setup and Launch Guide

This guide provides step-by-step instructions to set up and launch an STM32 project using STM32CubeIDE 1.9.0. 
It assumes that you have downloaded the project repository as a .zip file and have extracted the files.

## Prerequisites

Before proceeding, ensure that you have the following:

- STM32CubeIDE 1.9.0 installed on your computer. You can download it from the [STMicroelectronics website](https://www.st.com/en/development-tools/stm32cubeide.html).
- The project repository downloaded and extracted on your local machine.

## Project Setup

Follow these steps to set up the project in STM32CubeIDE:

1. Launch STM32CubeIDE 1.9.0 on your computer.

2. Choose a workspace where you want to store your project files. You can either select an existing workspace or create a new one.

3. Click on **File** in the menu bar, then select **Open Projects from File System**.

4. In the **Import** window, select **Directory**, and click **Browse**.

5. Navigate to the directory where you extracted the project files, select the project folder (LoRa_Steval or LoRa_Nucleo) and click **Select Folder**.

6. Ensure that the project you want to import is selected in the **Projects** section, and click **Finish**.

    a. For LoRa_Steval: 
        We want import only the third project named: \LoRa_Steval\Projects\STEVAL-STRKT01\Applications\LoRa\Asset_Tracker\SW4STM32\LoRa_STEVAL_STRKT01\


7. Wait for STM32CubeIDE to import the project. It may take a few moments to complete.

8. Once the import is finished, the project will appear in the **Project Explorer** view on the left-hand side of the IDE.

## Build and Launch

Follow these steps to build and launch the STM32 project:

1. Ensure that your STM32 microcontroller is connected to your computer via a suitable programming/debugging tool, such as an ST-Link or J-Link.

2. In the **Project Explorer** view, right-click on your project name and select **Build Project**. This will compile the source code and generate the necessary binary files.

3. After a successful build, right-click on your project name again, and this time select **Run As** > **STM32 Cortex-M C/C++ Application**.

    a. Keep in mind that for LoRa_Steval application, you should use the Nucleo board as Debugger (for more information read the section [3.4.4 Update via ST-LINK/V2 in-circuit debugger/programmer (page. 25/47)](https://www.st.com/resource/en/user_manual/um2541-getting-started-with-the-stevalstrkt01-lora-iot-tracker-stmicroelectronics.pdf)



