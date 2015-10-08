# WCM AWS IoT Starter Kit Out of Box Setup Guide

## Introduction
AWS has introduced a new service that will allow internet of things (IoT) devices to be easily and securely connected to their new AWS IoT service.

###Current version is 0.1.0

#### Notes About this version
This version of the software is being released as a __beta__ version.  Please check back often as there is going to be many updates to this starter kit.

### Required Tools and Applications
You will need the following Microchip development tools to compile and program the WCM AWS IoT Starter Kit

- Download and install Microchip MPLAB<sup>&reg;</sup>X IDE for your OS from here:
[Microchip MPLAB<sup>&reg;</sup>X](http://www.microchip.com/mplabx).

- Download and install the latest XC32 compiler for your OS from here:  [Microchip XC32](http://www.microchip.com/xc32)

	>NOTE: MPLAB® XC Compiler licenses are also available as Free editions and have the option of a 60-day PRO evaluation trial.  This will be required as the source code requires optimizations to be turned on.

- [PICKit&trade; 3 In-Circuit Debugger/Programmer](http://www.microchip.com/pickit3).  This programmer uses in-circuit debugging logic incorporated into each chip with Flash memory to provide a low-cost hardware debugger and programmer. You can use this programmer to load firmware into the WCM board.


#### Mac OS X Installation
This assumes that you have not installed previous version of awscli, or openssl; if you have you will need to uninstall those versions.  You will need to use a version of openssl 1.0.0 or later to connect to AWS IoT services as TLS 1.1 is required.  To do this follow the these steps:

1. Use [Homebrew](http://brew.sh/) to install the latest version of openssl with the following commands in a terminal window.  You will need to force the links so that Python will use the right version of openssl.

	```
	brew update
	brew install openssl
	brew link --force openssl
	```
- Reinstall Python to use the latest openssl:

	```
	brew install python --with-brewed-openssl
	```
- To verify that python was updated run the following command and make sure the version os greater than 1.0.2d

	```
	python -c 'import ssl; print ssl.OPENSSL_VERSION'
	```
- Install/upgrade the AWS CLI:

	```
	pip install --upgrade awscli
	```
- To verify the version of the AWS CLI tool installed run the following command and look for verison 1.8.12 or greater:

	```
	aws --version
	```

#### Windows Installation

1. Follow the instructions to install the [AWS CLI Using the MSI Installer](http://docs.aws.amazon.com/cli/latest/userguide/installing.html#install-msi-on-windows)
- Download and install the latest version of [Perl](https://www.perl.org/get.html)




## AWS IoT Service Setup
To use the AWS IoT services you will need to make use of awscli tool that you installed.  For more infomation on how to use this tool please go to [AWS Command Line Interface User Guide](http://docs.aws.amazon.com/cli/latest/userguide/cli-chap-welcome.html).

1. Create certificates that will be used in the application firmware by running the following command in your terminal/command prompt window:

	```
	aws iot create-keys-and-certificate --set-as-active
	```
	> You will need to copy the output on the terminal/command window to text files for the certs and private key.  NOTE: this is the only time that you will be able to see this information
- Create and attach the following policy that will allow your thing to publish and subscribe to the AWS IoT service
	- First create a file that will have the policy information in it and save it to you computer:

		```
		{
			"Version": "2012-10-17",
			"Statement": [{
				"Effect": "Allow",
				"Action":["iot:*"],
				"Resource": ["*"]
			}]
		}
		```
	- Load the policy to the AWS IoT services with the following command:

		```
		aws iot create-policy --policy-name PubSubToAnyTopic --policy-document file://<path-to-your-policy-document>
		```
	- You will need to attached this policy to a principal to allow the cert the rights to publish and subscribe to topics:

		```
		aws iot attach-principal-policy --principal-arn certificate-arn --policy-name PubSubToAnyTopic
		```
- Create a thing shadow; for this example we will use the id of 000001wcm:

	```
	aws iot create-thing --thing-name '000001wcm'
	```

## WCM Firmware Setup

### Certificate Conversion
You will have generated a client certificate and a client private key that will be used on the starter kit.  The certificate and key generated from the AWS IoT Service Setup section must be encoded in DER format and renamed as such:`clientKey.der`, `clientCert.der`. These must be converted to byte arrays so that it can be used by the board.  Place these certificates and key into the folder:

```
<path-to-your-project>/wcm-aws-thing-sdk-development/software/tcpip/wcm-aws-iot-sdk/firmware/src/certs
```

Next, to convert the certificate and key use the command `./gencertbuf.pl` in the directory.  This command will generate the certificates and key in byte arrays in a file called `certs.h`.  
>There is already a root CA that is used to authorize the AWS IoT service.  This cert is in the proper DER format and will be converted along with your client cert and client private key.

### Programming the WCM AWS IoT Starter Kit
1.  Open MPLAB<sup>&reg;</sup>X IDE
- Select `File->Open Project`
- Navigate to:

	```
	<path-to-your-project>/wcm-aws-iot-sdk/software/tcpip/wcm-aws-iot-sdk/firmware
	```
- Select the project `wcm-aws-iot-sdk.X`
- With the project open, we will modify the configuration to use your local wireless access point.  In the projects tab in the top left, expand the folders and double click on `system_config.h` to open the file.

	```
	wcm-aws-iot-sdk
	└-- Header Files
	    └-- app
	        └-- system_config
	            └-- wcm_dev_kit
	                └-- system_config.h
	```
- Find the definition `#define WF_DEFAULT_SSID_NAME` and change the text "Your AP Here" to the SSID of the local wireless access point you wish the board to connect to.
- Next, find the definition for `#define WF_DEFAULT_PSK_PHRASE"` and change the text from "Your Passphrase Here" to the passphrase of your local wireless access point.
	- If the network is an open network, then a password is not needed and you just need to change the definition `#WF_DEFAULT_WIFI_SECURITY_MODE` to `DRV_WIFI_SECURITY_OPEN`.

With the Wifi configuration in place, you are ready to program the board.  

1. Connect the PICKit&trade; 3 to your computer using the provided USB cable.  Connect the PICKit&trade; to the WCM AWS IoT Starter Kit ICSP 6-pin header.  The pins should match by aligning the triangles.  
- Power the WCM AWS IoT Starter Kit by connecting a Mini-USB cable to a power source and changing the power switch on the board to `USB`.
- On the top tab of MPLAB&reg; X, click the `Make and Program Device Main Project` button.  The button is a picture of a green down arrow pointing to a microcontroller.  Alternatively, you can right-click on the wcm-aws-iot-sdk in the `Projects` tab and select `Make and Program Device`.
- MPLAB&reg; X may prompt you to select with programmer to use.  Select PICKit&trade; 3 from the prompt and continue.
- Your starter kit will now be programmed

## Running the Demo

Power the WCM AWS IoT Starter Kit by connecting a Mini-USB cable to a power source and changing the power switch on the board to `USB`.

Once the software has been compiled and programed into the WCM AWS IoT stater Kit you can power on the device.  You will see a series of LED until the board has connected to the AP you setup in the firmware.  Now the fun begins as you can have complete control of the board.  This starter kit allows you to either read the state of the board (buttons, LEDs, and Pot) or set one of the 4 green LEDs (D1, D2, D3, & D4) to a __on__ or __off__ state.

### Reading the state of the starter kit

Using the AWS cli tools you can read the status of the board and see what state the buttons, LEDs, and pot.  To read the status of the board run the following command:

```
aws iot-data get-thing-shadow --thing-name 000001wcm output.txt && cat output.txt
```
> Note on windows you will have to put single quotes around the output file names to run both commands; like this ``` 'output.txt && cat output.txt' ```

The output from this will look like this very long string of text:

```
{"state":{"desired":{"led1":"on","led2":"off","led3":"off","led4":"off"},"reported":{"button1":"up","button2":"up","button3":"up","button4":"up","potentiometer":"   0","led1":"on","led2":"off","led3":"off","led4":"off"}},"metadata":{"desired":{"led1":{"timestamp":1444181344},"led2":{"timestamp":1444181326},"led3":{"timestamp":1444181328},"led4":{"timestamp":1444178525}},"reported":{"button1":{"timestamp":1444180604},"button2":{"timestamp":1444180604},"button3":{"timestamp":1444179853},"button4":{"timestamp":1444179876},"potentiometer":{"timestamp":1444175550},"led1":{"timestamp":1444183278},"led2":{"timestamp":1444183277},"led3":{"timestamp":1444183277},"led4":{"timestamp":1444183277}}},"version":16888,"timestamp":1444184353}
```
Not very readable.  To make this more readable you can copy these values into a web utility like [JSON Pretty Print](http://jsonprettyprint.com). Below is an example of the the formatted output:

```
{
  "state": {
    "desired": {
      "led1": "on",
      "led2": "off",
      "led3": "off",
      "led4": "off"
    },
    "reported": {
      "button1": "up",
      "button2": "up",
      "button3": "up",
      "button4": "up",
      "potentiometer": "   0",
      "led1": "on",
      "led2": "off",
      "led3": "off",
      "led4": "off"
    }
  },
  "metadata": {
    "desired": {
      "led1": {
        "timestamp": 1444181344
      },
      "led2": {
        "timestamp": 1444181326
      },
      "led3": {
        "timestamp": 1444181328
      },
      "led4": {
        "timestamp": 1444178525
      }
    },
    "reported": {
      "button1": {
        "timestamp": 1444180604
      },
      "button2": {
        "timestamp": 1444180604
      },
      "button3": {
        "timestamp": 1444179853
      },
      "button4": {
        "timestamp": 1444179876
      },
      "potentiometer": {
        "timestamp": 1444175550
      },
      "led1": {
        "timestamp": 1444183278
      },
      "led2": {
        "timestamp": 1444183277
      },
      "led3": {
        "timestamp": 1444183277
      },
      "led4": {
        "timestamp": 1444183277
      }
    }
  },
  "version": 16888,
  "timestamp": 1444184353
}
```

There is a lot of information in the contained in the output text file.  The items to be focused on are the ones in the reported state section, this will tell you what the starter kit last reported to the AWS IoT service

### Setting the state of the starter kit

Now that you have read the status of the starter kit, how about we set the value of one of the LEDs to the _on_ state.  This is done with the following command:

```
aws iot-data update-thing-shadow --thing-name 000001wcm --payload '{"state":{"desired":{"led2":"on"}}}' output.txt && cat output.txt
```
> Note on windows you will have to put single quotes around the output file names to run both commands; like this ``` 'output.txt && cat output.txt' ```

The AWS IoT will return a JSON payload that show you have changed the desired state of the object, again this is a string that will be to be run through a tool that will make it more readable as shown below:

```
{
  "state": {
    "desired": {
      "led2": "on"
    }
  },
  "metadata": {
    "desired": {
      "led2": {
        "timestamp": 1444184928
      }
    }
  },
  "version": 16905,
  "timestamp": 1444184928
}
```

## License
© 2015 Microchip Technology Inc. and its subsidiaries.  You may use this
software and any derivatives exclusively with Microchip products.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE, OR
ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR
USE IN ANY APPLICATION.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED
BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY
TO MICROCHIP FOR THIS SOFTWARE.

MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
TERMS.
