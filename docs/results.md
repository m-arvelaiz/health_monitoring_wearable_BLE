# BLE implementation

The BLE profile described before was included in the PoF device, using the STM32WB5MMGH6TR, the implementation included its characteristic, services and attributes  to be able to establish the communication with the aplication and exchange data. The final implementation can be seen while using a BLE debugging application for mobiles (ex: eDebugger), and the final profile is the one showed in the images below:

***Include captures from edebugger***

as mentioned the to encrypt the communication and tackle evadropiing and related MITM attacks in the first pairing some key are exchaged using the pairing ----, that encrypt the communication.


To evaluate how this works two test were done one in which the device does not implement this kind of secure pairing and one in which it does.

For that the setup of the test the device is communicating with a mobile phone that send a cmd to retrive HR messure, using the cmd was done usinf wireshark and some sniifing tools

In the first figures are captures of the command send from a mobile device using the eDebugger application and the other capture belongs to a capture of wireshark in which a MITM can evadrop all the information being exchanged.

***Capture of the edebugger sending the cmd and reciving the response and a capture of the evadropp package***
For the second test the secure pairing was activated and the the result are:
***Capture of the edebugger sending the cmd and reciving the response and a capture of the evadropp package appling security***

In this case the imamages above we can see that the mobile deve send and recive the data as expected but the wireshark acting as MITM see the package as encrypted, which is a proof of how this pairing helps in the data protection of the data.

