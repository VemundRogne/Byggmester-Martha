# TTK4155: Byggmester-Martha
> We're playing ping-pong!

## Node 1
The code for Node1 is contained in the folder "ByggmesterMartha" and is an Atmel-Studio project.

## Node 2
The code for node 2 is contained in the folder "Node2" and is an Atmel-Studio project.

## Continous Integration and automated testing
To ease the development process we setup an automated CI and testing framework through Github actions. The full action is described in ".github/Node1.yml"

Every time a new commit is pushed to github this automated system:
1. Downloads the new code in a fresh environment
2. Builds node 1 and node 2
3. Flashes the newly build node 1 and node 2 (if it did actually sucessfully build)
4. Run tests on the hardware through the communications-interface

### Test examples
A nice example of a test is the test_can::test_can test. This test sets the MCP2515 into loopback mode, constructs and sends a can message and verifies that the same message is read back. While performing this test the comms inteface calls the same functions that the firmware would, and thus proves that the entire can-chain is valid.

Another nice example is the tests we run on the motor. Here we run the motor in both directions while reading encoder values, then we ensure that the encoder values are monotonic (while the motor is moving left the encoder values should increase, while the motor is moving right the encoder values should decrease). Finally we call the regulator-init function and verify that the initialization was successful and that the final motor-position regulates to the middle.

## Communications-interface
We have developed a protocol for communication between a host computer and node 1. The protocol makes it possible to get and set variables, run tests and collect debug-information.

### Installation
```
pip install .
```
### Example
Let's send a CAN-message to Node2.
```python
# Import our custom communications-protocol
import byggern

# Open a serial connection
serial_connection = byggern.comms.open_serial_connection(COMPORT)

# Send a can message
data = [1, 2, 3, 4, 5]  # Arbitrary data
msg_id = 1000           # Arbitrary ID
byggern.can_transmit(
    serial_connection,
    msg_id = msg_id,
    msg_len = len(data),
    msg_data = data
)
```
