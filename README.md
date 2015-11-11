### IoT Wi-Fi G AWS IoT Starter Kit
#### Out of Box Setup Guide

---
### Introduction
AWS has introduced a new service that will allow internet of things (IoT) devices to be easily and securely connected to their new [AWS IoT](http://aws.amazon.com/iot/) service.

#### Notes About This Version
This version of the software is being released as a __beta__ version.  Please check back often as there will be many updates to this starter kit.  This version of the starter kit has been based on our [WCM (Wi-Fi Client Module) Development Kit 1](http://www.microchipdirect.com/ProductSearch.aspx?keywords=DM182020).  If you already have one of these kits you can use it for running the demo listed below.

#### Microchip Required Tools and Applications
You will need the following Microchip development tools to run the following out of box demo

- Order the [IoT Wi-Fi G AWS IoT Starter Kit](http://www.microchipdirect.com/productsearch.aspx?Keywords=DM990001) from Microchip Direct.  This starter kit includes PICKit&trade; 3 In-Circuit Debugger/Programmer
  >NOTE: If this tool is not available you can order the WCM (Wi-Fi Client Module) Development Kit 1 above and the PICKit&trade; 3 In-Circuit Debugger/Programmer from Microchip Direct

- Download and install Microchip MPLAB<sup>&reg;</sup>X IDE for your OS from here:
[Microchip MPLAB<sup>&reg;</sup> X IDE](http://www.microchip.com/mplabx)

- Download and install the latest XC32 compiler for your OS from here: [Microchip MPLAB<sup>&reg;</sup> XC32 Compiler](http://www.microchip.com/xc32)
  >NOTE: MPLAB<sup>&reg;</sup> XC Compiler licenses are also available as Free editions and have the option of a 60-day PRO evaluation trial.  This will be required as the source code requires optimizations to be turned on.

#### AWS Required Applications
To use the AWS IoT services you will need to make use of _AWS Command Line Interface_ (awscli) tool.  For more information on how to install, configure, and use the awscli tool please go to [AWS Command Line Interface User Guide](http://docs.aws.amazon.com/cli/latest/userguide/cli-chap-welcome.html).

#### Linux, Unix, and Windows Installations
Follow the instructions in the [Getting Set Up with the AWS Command Line Interface](http://docs.aws.amazon.com/cli/latest/userguide/cli-chap-getting-set-up.html) for installations for your operating system.

#### Mac OS X Installation
This assumes that you have not installed previous version of Python, awscli, or openssl; if you have you will need to uninstall those versions.  You will need to use a version of openssl 1.0.0 or later to connect to AWS IoT services as TLS 1.1 is required.  To do this follow the these steps:

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

---
### AWS IoT Service Setup
To setup your AWS IoT service to work with the _IoT Wi-Fi G AWS IoT Starter Kit_ you will need to work with the awscli that you installed in the previous section.

1. Using the awscli you will need to find the endpoint that will be used for your AWS IoT account.  To do this run the following command in your terminal/command window.

  ```
  aws iot describe-endpoint
  ```
You will receive the following back that you will need to add to the firmware in the next section.  The output will look like: `<random-string>.iot.us-east-1.amazonaws.com`
- Create certificates that will be used in the application firmware by running the following command in your terminal/command prompt window:

  ```
  aws iot create-keys-and-certificate --set-as-active
  ```
  > You will need to copy the output on the terminal/command window to text files for the certs and private key.
  > NOTE: this is the only time that you will be able to see this information
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
- Create a thing shadow; for this example we will use the id of 000001iot:

  ```
  aws iot create-thing --thing-name '000001iot'
  ```

---
### IoT Wi-Fi G AWS IoT Starter Kit Firmware Setup

#### Certificate Conversion
You will have generated a client certificate and a client private key that will be used on the starter kit.  The certificate and key generated from the AWS IoT Service Setup section must be encoded in DER format and renamed as such: _clientKey.der_, _clientCert.der_. These must be converted to byte arrays so that it can be used by the board.

1. Place these certificates and key into the folder:

  ```
  <path-to-your-project>/wcm-aws-thing-sdk/software/tcpip/wcm-aws-iot-sdk/firmware/src/certs
  ```
- Convert the certificate and key by using the command `./gencertbuf.pl` in the directory.  This command will generate the certificates and key in byte arrays in a file called _certs.h_.

  >There is already a root CA that is used to establish the SSL/TLS connection to the AWS IoT service.  This cert is in the proper DER format and will be converted along with your client cert and client private key.

#### Programming the IoT Wi-Fi G AWS IoT Starter Kit
To get your starter kit up and running, you will have to program the starter kit with your generated certificates.

1.  Open MPLAB<sup>&reg;</sup> X IDE
- Select __File -> Open Project__
- Navigate to: `  <path-to-your-project>/iot-wifi-g-aws-iot-sdk/software/tcpip/wcm-aws-iot-sdk/firmware`
- Select the project __iot-wifi-g-aws-iot-sdk.X__
- Connect the PICKit&trade; 3 to your computer using the provided USB cable.
- Connect the PICKit&trade; 3 to the IoT Wi-Fi G AWS IoT Starter Kit ICSP 6-pin header.  The pins should match by aligning the triangles.
- Power on the starter kit by connecting a Mini-USB cable to a power source and changing the power switch on the board to USB.
- On the top tab of MPLAB&reg; X, click the __Make and Program Device Main Project__ button.  ![](/images/program_icon.png)

  >MPLAB&reg; X may prompt you to select with programmer to use.  Select PICKit&trade; 3 from the prompt and continue.

---
### Running the Demo

#### Commissioning the Demo

To run the demo follow these instructions:

1. Power on the IoT Wi-FI G AWS IoT Starter Kit by connecting a Mini-USB cable to a power source and changing the power switch on the board to USB
- The starter kit will create an access point (AP) with an SSID of __xxxxxx_IoT__ where __xxxxxx__ is the last six characters of your starter kit's MAC address
- Connect to this SSID using your computer or mobile device.
- Once connected open a browser and enter the IP address `192.168.1.1`
  - This IP address will bring you to the configuration webpage for your starter kit
- On the configuration webpage enter your local network SSID into the box labeled Network SSID
- Select a security type for your local network from the drop down box labeled Security Type
  > If a security type of _WEP_ or _WPA/WPA2_ is selected, a box will appear prompting for a password
- Enter your AWS IoT Endpoint that you received from the previous AWS IoT Service Setup section
  - The endpoint will have the form `<random-string>.iot.us-east-1.amazonaws.com`
- Click the __Join__ button and a _Reconnection in Progress_ page will appear showing the network that the starter kit will reconnect to and your AWS IoT Endpoint.

You will see a series of LEDs flash until the board has connected to the AP you setup.  When the starter kit has connected you should see the D6 LED flickering with network traffic letting you know that you can read and set the state of the starter kit as described in the next section.

> NOTE: If you need to change the local network and AWS IoT Endpoint you entered, or if you entered it incorrectly and need to redo the process, simply power off the starter kit, press and hold S2 and S3 while turning on the power to the starter kit.  This will erase the current configuration and you can begin this process again and re-enter the network information and AWS IoT Endpoint.

#### Reading the State of the Starter Kit

Using the AWS cli tools you can read the status of the board and see what state the buttons, LEDs, and pot.  To read the status of the board run the following command:

```
aws iot-data get-thing-shadow --thing-name 000001iot output.txt && cat output.txt
```
> NOTE: on windows you will have to put single quotes around the output file names to run both commands; like this ``` 'output.txt && cat output.txt' ```

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

#### Setting the State of the Starter Kit

Now that you have read the status of the starter kit, how about we set the value of one of the LEDs to the _on_ state.  This is done with the following command:

```
aws iot-data update-thing-shadow --thing-name 000001iot --payload '{"state":{"desired":{"led2":"on"}}}' output.txt && cat output.txt
```

> NOTE: on windows you will have to put single quotes around the output file names to run both commands; like this ``` 'output.txt && cat output.txt' ```

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
