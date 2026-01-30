/**
 * MEDIATOR PATTERN
 *
 * Intent: Define an object that encapsulates how a set of objects interact.
 * Promotes loose coupling by keeping objects from referring to each other explicitly.
 *
 * Problem: Multiple objects interact, creating complex interdependencies
 * Solution: Create mediator that handles all communication between objects
 *
 * Real-world analogy: Air traffic controller mediates communication between planes
 *
 * SOLID relation:
 * - SRP: Mediator centralizes interaction logic
 * - DIP: Colleagues depend on Mediator abstraction
 */

#include <iostream>
#include <memory>
#include <string>
#include <map>

// ============================================================================
// EXAMPLE 1: Chat Room Mediator
// ============================================================================

// PROBLEM: Direct communication between users creates tight coupling
// ---
// Without Mediator:
// ✗ Each user knows about other users
// ✗ User A sends to User B, User B sends to User C, etc.
// ✗ Complex web of dependencies
// ✗ Hard to add new users
// ✗ Difficult to manage user list
// ✗ Violates SRP and DIP
//
// Example messy WITHOUT Mediator:
//   User alice("Alice");
//   User bob("Bob");
//   User charlie("Charlie");
//
//   void sendMessage(User& from, User& to, string msg) {
//       to.receive(from.name, msg);
//   }
//
//   // Everyone must know everyone
//   alice.send(bob, "Hi Bob");  // tight coupling
//   bob.send(charlie, "Hi Charlie");  // tight coupling
//   // Adding new user? All existing users must know!

// SOLUTION: Mediator centralizes all communication
// ---
// Key insight: Users don't communicate directly; they use mediator
// Mediator knows all users and routes messages
//
// Benefits:
// ✓ Loose coupling - users don't know each other
// ✓ Centralized communication logic
// ✓ Easy to add new users
// ✓ Easy to monitor/log all messages
// ✓ Easy to add restrictions (ignore lists, etc.)
// ✓ DIP - users depend on mediator interface

class ChatRoomMediator;

class User
{
protected:
    std::string name_;
    ChatRoomMediator *chatRoom_;

public:
    User(const std::string &name) : name_(name), chatRoom_(nullptr) {}
    virtual ~User() = default;

    virtual void send(const std::string &to, const std::string &message) = 0;
    virtual void receive(const std::string &from, const std::string &message) = 0;

    std::string getName() const { return name_; }
    void setChatRoom(ChatRoomMediator *room) { chatRoom_ = room; }
};

// Mediator interface
class ChatRoomMediator
{
public:
    virtual ~ChatRoomMediator() = default;
    virtual void registerUser(std::shared_ptr<User> user) = 0;
    virtual void sendMessage(const std::string &from, const std::string &to,
                             const std::string &message) = 0;
    virtual void broadcastMessage(const std::string &from,
                                  const std::string &message) = 0;
};

// Concrete User
class ChatUser : public User
{
public:
    ChatUser(const std::string &name) : User(name) {}

    void send(const std::string &to, const std::string &message) override
    {
        std::cout << "[" << name_ << "] sending to [" << to << "]: " << message << "\n";
        chatRoom_->sendMessage(name_, to, message);
    }

    void receive(const std::string &from, const std::string &message) override
    {
        std::cout << "[" << name_ << "] received from [" << from << "]: " << message << "\n";
    }

    void broadcast(const std::string &message)
    {
        std::cout << "[" << name_ << "] broadcasting: " << message << "\n";
        chatRoom_->broadcastMessage(name_, message);
    }
};

// Concrete Mediator
class ChatRoom : public ChatRoomMediator
{
private:
    std::map<std::string, std::shared_ptr<User>> users_;

public:
    void registerUser(std::shared_ptr<User> user) override
    {
        users_[user->getName()] = user;
        user->setChatRoom(this);
        std::cout << "[ChatRoom] " << user->getName() << " joined (now "
                  << users_.size() << " users)\n";
    }

    void sendMessage(const std::string &from, const std::string &to,
                     const std::string &message) override
    {
        if (users_.find(to) != users_.end())
        {
            users_[to]->receive(from, message);
        }
        else
        {
            std::cout << "[ChatRoom] ERROR: User " << to << " not found\n";
        }
    }

    void broadcastMessage(const std::string &from,
                          const std::string &message) override
    {
        for (auto &[name, user] : users_)
        {
            if (name != from)
            {
                user->receive(from, message);
            }
        }
    }
};

// ============================================================================
// EXAMPLE 2: Dialog Box with Complex Interactions
// ============================================================================

// PROBLEM: UI components have complex interdependencies
// ---
// Dialog: TextField, ComboBox, Buttons
// When user changes ComboBox:
// - Enable/disable Button1 based on selection
// - Update TextField based on ComboBox value
// - Enable/disable Button2 based on TextField content
//
// SOLUTION: Dialog acts as mediator for UI components

class DialogMediator;

class UIComponent
{
protected:
    DialogMediator *dialog_;

public:
    virtual ~UIComponent() = default;
    void setDialog(DialogMediator *d) { dialog_ = d; }
    virtual void changed() = 0;
    virtual std::string getState() const = 0;
};

class DialogMediator
{
public:
    virtual ~DialogMediator() = default;
    virtual void componentChanged(UIComponent *component) = 0;
};

class TextFieldComponent : public UIComponent
{
private:
    std::string value_;

public:
    void setValue(const std::string &v)
    {
        std::cout << "  [TextField] Input: " << v << "\n";
        value_ = v;
        changed();
    }

    void changed() override
    {
        if (dialog_)
        {
            dialog_->componentChanged(this);
        }
    }

    std::string getState() const override { return value_; }
};

class ComboBoxComponent : public UIComponent
{
private:
    std::string selected_;

public:
    void select(const std::string &option)
    {
        std::cout << "  [ComboBox] Selected: " << option << "\n";
        selected_ = option;
        changed();
    }

    void changed() override
    {
        if (dialog_)
        {
            dialog_->componentChanged(this);
        }
    }

    std::string getState() const override { return selected_; }
};

class ButtonComponent : public UIComponent
{
private:
    std::string label_;
    bool enabled_ = true;

public:
    ButtonComponent(const std::string &label) : label_(label) {}

    void setEnabled(bool enabled)
    {
        if (enabled != enabled_)
        {
            enabled_ = enabled;
            std::cout << "  [Button] " << label_ << " "
                      << (enabled ? "ENABLED" : "DISABLED") << "\n";
        }
    }

    bool isEnabled() const { return enabled_; }
    void click()
    {
        std::cout << "  [Button] " << label_ << " clicked\n";
    }

    void changed() override {}
    std::string getState() const override { return label_; }
};

class RegistrationDialog : public DialogMediator
{
private:
    std::shared_ptr<TextFieldComponent> emailField_;
    std::shared_ptr<ComboBoxComponent> countryBox_;
    std::shared_ptr<ButtonComponent> okButton_;

public:
    RegistrationDialog()
    {
        emailField_ = std::make_shared<TextFieldComponent>();
        countryBox_ = std::make_shared<ComboBoxComponent>();
        okButton_ = std::make_shared<ButtonComponent>("OK");

        emailField_->setDialog(this);
        countryBox_->setDialog(this);
        okButton_->setDialog(this);
    }

    void componentChanged(UIComponent *component) override
    {
        std::cout << "[Dialog] Component changed, updating state:\n";

        // Complex logic coordinating components
        bool emailValid = !emailField_->getState().empty();
        bool countrySelected = !countryBox_->getState().empty();

        okButton_->setEnabled(emailValid && countrySelected);

        if (countryBox_->getState() == "USA")
        {
            std::cout << "  [Dialog] US selected - applying country-specific rules\n";
        }
    }

    TextFieldComponent &getEmailField() { return *emailField_; }
    ComboBoxComponent &getCountryBox() { return *countryBox_; }
    ButtonComponent &getOKButton() { return *okButton_; }
};

// ============================================================================
// EXAMPLE 3: Air Traffic Control
// ============================================================================

// PROBLEM: Planes need to communicate flight plans and avoid collisions
// ---
// Without Mediator: Each plane tracks all other planes
// With Mediator: Control tower mediates all communication

class AirTrafficControlTower;

class Aircraft
{
protected:
    std::string callSign_;
    AirTrafficControlTower *tower_;
    int altitude_ = 0;

public:
    Aircraft(const std::string &call) : callSign_(call), tower_(nullptr) {}
    virtual ~Aircraft() = default;

    virtual void requestLanding() = 0;
    virtual void receiveInstruction(const std::string &instruction) = 0;

    std::string getCallSign() const { return callSign_; }
    void setTower(AirTrafficControlTower *t) { tower_ = t; }
};

class AirTrafficControlTower
{
public:
    virtual ~AirTrafficControlTower() = default;
    virtual void registerAircraft(std::shared_ptr<Aircraft> aircraft) = 0;
    virtual void requestLanding(const std::string &callSign) = 0;
};

class Plane : public Aircraft
{
public:
    Plane(const std::string &call) : Aircraft(call) {}

    void requestLanding() override
    {
        std::cout << "[" << callSign_ << "] Requesting landing clearance\n";
        tower_->requestLanding(callSign_);
    }

    void receiveInstruction(const std::string &instruction) override
    {
        std::cout << "[" << callSign_ << "] Instruction: " << instruction << "\n";
    }
};

class Tower : public AirTrafficControlTower
{
private:
    std::map<std::string, std::shared_ptr<Aircraft>> aircraft_;
    std::string runwayStatus_ = "CLEAR";

public:
    void registerAircraft(std::shared_ptr<Aircraft> aircraft) override
    {
        aircraft_[aircraft->getCallSign()] = aircraft;
        aircraft->setTower(this);
        std::cout << "[Tower] " << aircraft->getCallSign() << " checked in\n";
    }

    void requestLanding(const std::string &callSign) override
    {
        std::cout << "[Tower] Processing landing request from " << callSign << "\n";

        if (runwayStatus_ == "CLEAR")
        {
            runwayStatus_ = "OCCUPIED";
            aircraft_[callSign]->receiveInstruction("Clear to land on runway 1");
            std::cout << "[Tower] Runway now OCCUPIED\n";
        }
        else
        {
            aircraft_[callSign]->receiveInstruction("Hold position, runway occupied");
            std::cout << "[Tower] Runway still OCCUPIED\n";
        }
    }

    void clearRunway()
    {
        runwayStatus_ = "CLEAR";
        std::cout << "[Tower] Runway cleared for next aircraft\n";
    }
};

// ============================================================================
// Demonstration
// ============================================================================

int main()
{
    std::cout << "=== MEDIATOR PATTERN DEMO ===\n";

    // Chat room
    std::cout << "\n--- Chat Room Mediator ---\n";
    {
        auto chatRoom = std::make_shared<ChatRoom>();

        auto alice = std::make_shared<ChatUser>("Alice");
        auto bob = std::make_shared<ChatUser>("Bob");
        auto charlie = std::make_shared<ChatUser>("Charlie");

        chatRoom->registerUser(alice);
        chatRoom->registerUser(bob);
        chatRoom->registerUser(charlie);

        std::cout << "\nDirect messages:\n";
        std::static_pointer_cast<ChatUser>(alice)->send("Bob", "Hi Bob!");
        std::static_pointer_cast<ChatUser>(bob)->send("Charlie", "Hey Charlie!");

        std::cout << "\nBroadcast message:\n";
        std::static_pointer_cast<ChatUser>(alice)->broadcast("Hello everyone!");
    }

    // Dialog with UI components
    std::cout << "\n--- Dialog Mediator ---\n";
    {
        RegistrationDialog dialog;

        std::cout << "User interactions:\n";
        dialog.getEmailField().setValue("user@example.com");
        dialog.getCountryBox().select("USA");

        std::cout << "\nTrying to submit:\n";
        if (dialog.getOKButton().isEnabled())
        {
            dialog.getOKButton().click();
        }
        else
        {
            std::cout << "  [Dialog] Cannot click - button is disabled\n";
        }
    }

    // Air traffic control
    std::cout << "\n--- Air Traffic Control ---\n";
    {
        auto tower = std::make_shared<Tower>();

        auto flight1 = std::make_shared<Plane>("AA101");
        auto flight2 = std::make_shared<Plane>("UA202");

        tower->registerAircraft(flight1);
        tower->registerAircraft(flight2);

        std::cout << "\nFlight 1 landing request:\n";
        flight1->requestLanding();

        std::cout << "\nFlight 2 landing request (runway occupied):\n";
        flight2->requestLanding();

        std::cout << "\nClearing runway:\n";
        tower->clearRunway();

        std::cout << "Flight 2 landing request (runway now clear):\n";
        flight2->requestLanding();
    }

    std::cout << "\n=== KEY POINTS ===\n";
    std::cout << "1. Mediator centralizes object interactions\n";
    std::cout << "2. Objects communicate through mediator\n";
    std::cout << "3. Loose coupling between objects\n";
    std::cout << "4. Complex interaction logic in one place\n";
    std::cout << "5. Easy to modify interaction rules\n";
    std::cout << "6. Easy to monitor/log interactions\n";

    return 0;
}

/**
 * WHEN TO USE:
 * - Objects have complex interactions
 * - Direct object references create tight coupling
 * - Reusability of objects hindered by dependencies
 * - Interaction logic is complex and scattered
 * - Need to control and monitor object interactions
 * - Want to centralize communication rules
 *
 * BENEFITS:
 * - Decouples objects (don't reference each other)
 * - Centralizes interaction logic
 * - Easy to change interaction rules
 * - Reusability improved (objects independent)
 * - SRP - mediator handles interaction
 * - Easy to test interactions
 *
 * DRAWBACKS:
 * - Mediator can become complex ("God Object")
 * - Mediator may end up knowing too much
 * - Adds indirection (more method calls)
 * - Hard to debug interactions
 *
 * COMPARISON:
 * - Mediator: Centralizes object interactions
 * - Observer: One-to-many, loosely coupled notifications
 * - Facade: Simplifies subsystem interface
 * - Command: Encapsulates request
 *
 * VARIATIONS:
 * - Hierarchical mediators
 * - Mediator registry
 * - Mediator factory
 */
