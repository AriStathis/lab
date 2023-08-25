#include <iostream>
#include <vector>

enum class LogicLevel {
    Undefined = -1,
    Low,
    High
};

class Wire;

class Gate {
public:
    virtual LogicLevel GetOutput() const = 0;
    virtual void ConnectOutput(Wire* outputWire) = 0;
    virtual void DriveInput(int inputIndex, LogicLevel newLevel) = 0;
};

class Wire {
public:
    Wire() : outputGate(nullptr) {}

    void AddOutputConnection(Gate* gate, int inputIndex) {
        outputGate = gate;
        gateInputIndex = inputIndex;
    }

    void Drive(LogicLevel newLevel) {
        if (outputGate) {
            outputGate->DriveInput(gateInputIndex, newLevel);
        }
    }

private:
    Gate* outputGate;
    int gateInputIndex;
};

class XORGate : public Gate {
public:
    XORGate() : inputA(LogicLevel::Undefined), inputB(LogicLevel::Undefined) {}

    LogicLevel GetOutput() const override {
        if (inputA == LogicLevel::Undefined || inputB == LogicLevel::Undefined) {
            return LogicLevel::Undefined;
        }
        return (inputA != inputB) ? LogicLevel::High : LogicLevel::Low;
    }

    void ConnectOutput(Wire* outputWire) override {
        outputWire->AddOutputConnection(this, 0);
    }

    void DriveInput(int inputIndex, LogicLevel newLevel) override {
        if (inputIndex == 0) {
            inputA = newLevel;
        } else if (inputIndex == 1) {
            inputB = newLevel;
        }
    }

private:
    LogicLevel inputA;
    LogicLevel inputB;
};

class ANDGate : public Gate {
public:
    ANDGate() : inputA(LogicLevel::Undefined), inputB(LogicLevel::Undefined) {}

    LogicLevel GetOutput() const override {
        if (inputA == LogicLevel::Undefined || inputB == LogicLevel::Undefined) {
            return LogicLevel::Undefined;
        }
        return (inputA == LogicLevel::High && inputB == LogicLevel::High) ? LogicLevel::High : LogicLevel::Low;
    }

    void ConnectOutput(Wire* outputWire) override {
        outputWire->AddOutputConnection(this, 0);
    }

    void DriveInput(int inputIndex, LogicLevel newLevel) override {
        if (inputIndex == 0) {
            inputA = newLevel;
        } else if (inputIndex == 1) {
            inputB = newLevel;
        }
    }

private:
    LogicLevel inputA;
    LogicLevel inputB;
};

class ORGate : public Gate {
public:
    ORGate() : inputA(LogicLevel::Undefined), inputB(LogicLevel::Undefined) {}

    LogicLevel GetOutput() const override {
        if (inputA == LogicLevel::Undefined || inputB == LogicLevel::Undefined) {
            return LogicLevel::Undefined;
        }
        return (inputA == LogicLevel::High || inputB == LogicLevel::High) ? LogicLevel::High : LogicLevel::Low;
    }

    void ConnectOutput(Wire* outputWire) override {
        outputWire->AddOutputConnection(this, 0);
    }

    void DriveInput(int inputIndex, LogicLevel newLevel) override {
        if (inputIndex == 0) {
            inputA = newLevel;
        } else if (inputIndex == 1) {
            inputB = newLevel;
        }
    }

private:
    LogicLevel inputA;
    LogicLevel inputB;
};

class HalfAdder {
public:
    HalfAdder(Wire& inputA, Wire& inputB, Wire& sum, Wire& carry) : sum(sum), carry(carry) {
        XORGate* xorGate = new XORGate();
        ANDGate* andGate = new ANDGate();

        inputA.AddOutputConnection(xorGate, 0);
        inputB.AddOutputConnection(xorGate, 1);

        xorGate->ConnectOutput(&sum);
        inputA.AddOutputConnection(andGate, 0);
        inputB.AddOutputConnection(andGate, 1);

        andGate->ConnectOutput(&carry);
    }

private:
    Wire& sum;
    Wire& carry;
};

class FullAdder {
public:
    FullAdder(Wire& inputA, Wire& inputB, Wire& carryIn, Wire& sum, Wire& carryOut) : sum(sum), carryOut(carryOut) {
        XORGate* xorGate1 = new XORGate();
        XORGate* xorGate2 = new XORGate();
        ANDGate* andGate1 = new ANDGate();
        ANDGate* andGate2 = new ANDGate();
        ORGate* orGate = new ORGate();

        inputA.AddOutputConnection(xorGate1, 0);
        inputB.AddOutputConnection(xorGate1, 1);

        xorGate1->ConnectOutput(xorGate2, 0);
        carryIn.AddOutputConnection(xorGate2, 1);

        xorGate1->ConnectOutput(&sum);
        inputA.AddOutputConnection(andGate1, 0);
        inputB.AddOutputConnection(andGate1, 1);

        xorGate2->ConnectOutput(andGate2, 0);
        xorGate2->ConnectOutput(andGate1, 1);

        andGate1->ConnectOutput(&carryOut);
        andGate2->ConnectOutput(orGate, 0);
        carryIn.AddOutputConnection(andGate2, 1);

        orGate->ConnectOutput(&carryOut);
    }

private:
    Wire& sum;
    Wire& carryOut;
};

class BinaryAdder {
public:
    BinaryAdder(Wire inputsA[3], Wire inputsB[3], Wire outputs[4]) {
        Wire carry1, carry2, carry3;

        HalfAdder ha1(inputsA[0], inputsB[0], outputs[0], carry1);
        FullAdder fa1(inputsA[1], inputsB[1], carry1, outputs[1], carry2);
        FullAdder fa2(inputsA[2], inputsB[2], carry2, outputs[2], carry3);
        outputs[3].Drive(carry3);
    }
};

int main() {
    Wire inputsA[3], inputsB[3], outputs[4];

    BinaryAdder adder(inputsA, inputsB, outputs);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 3; k++) {
                inputsA[k].Drive(static_cast<LogicLevel>((i >> k) & 1));
                inputsB[k].Drive(static_cast<LogicLevel>((j >> k) & 1));
            }

            std::cout << "Input A: " << static_cast<int>(inputsA[2].GetOutput()) << static_cast<int>(inputsA[1].GetOutput()) << static_cast<int>(inputsA[0].GetOutput()) << " ";
            std::cout << "Input B: " << static_cast<int>(inputsB[2].GetOutput()) << static_cast<int>(inputsB[1].GetOutput()) << static_cast<int>(inputsB[0].GetOutput()) << " ";
            std::cout << "Sum: " << static_cast<int>(outputs[3].GetOutput()) << static_cast<int>(outputs[2].GetOutput()) << static_cast<int>(outputs[1].GetOutput()) << static_cast<int>(outputs[0].GetOutput()) << std::endl;
        }
    }

    return 0;
}
