/**
 * OBSERVER PATTERN (Pub-Sub Pattern)
 *
 * Intent: Define a one-to-many dependency between objects so that when one
 *         object changes state, all its dependents are notified automatically.
 *
 * Also Known As: Dependents, Publish-Subscribe
 *
 * When to Use:
 * - An abstraction has two aspects, one dependent on the other
 * - A change to one object requires changing others (unknown number)
 * - An object should notify other objects without assumptions about them
 *
 * Common Use Cases:
 * - Event handling systems (UI frameworks)
 * - MVC architecture (Model notifies Views)
 * - Real-time data feeds (stock prices, news)
 * - Notification systems
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

// ============================================================================
// PROBLEM: Tight coupling and polling
// ============================================================================

namespace problem_polling
{
    class WeatherStation
    {
    private:
        double temperature;
        double humidity;
        double pressure;

    public:
        WeatherStation() : temperature(25.0), humidity(60.0), pressure(1013.0) {}

        void setMeasurements(double temp, double hum, double press)
        {
            temperature = temp;
            humidity = hum;
            pressure = press;
            cout << "Weather data updated!\n";
        }

        double getTemperature() const { return temperature; }
        double getHumidity() const { return humidity; }
        double getPressure() const { return pressure; }
    };

    class Display
    {
    private:
        string name;

    public:
        Display(const string &n) : name(n) {}

        // Problem: Must actively poll for changes
        void checkAndUpdate(const WeatherStation &station)
        {
            cout << "[" << name << "] Polling weather station...\n";
            cout << "  Temp: " << station.getTemperature() << "°C\n";
            cout << "  Humidity: " << station.getHumidity() << "%\n";
        }
    };

    void demonstrate()
    {
        cout << "=== PROBLEM: Polling Approach ===\n";

        WeatherStation station;
        Display display1("Main Display");
        Display display2("Phone Display");

        station.setMeasurements(28.0, 65.0, 1010.0);

        // Problem: Displays must actively poll
        display1.checkAndUpdate(station);
        display2.checkAndUpdate(station);

        cout << "\nProblems:\n";
        cout << "- Tight coupling between station and displays\n";
        cout << "- Inefficient (constant polling)\n";
        cout << "- Displays might miss updates\n";
        cout << "- Station knows about all displays (not scalable)\n";
    }
}

// ============================================================================
// SOLUTION: Observer Pattern
// ============================================================================

namespace observer_solution
{
    // Observer interface
    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void update(double temperature, double humidity, double pressure) = 0;
    };

    // Subject interface
    class Subject
    {
    public:
        virtual ~Subject() = default;
        virtual void attach(Observer *observer) = 0;
        virtual void detach(Observer *observer) = 0;
        virtual void notify() = 0;
    };

    // Concrete Subject
    class WeatherStation : public Subject
    {
    private:
        vector<Observer *> observers;
        double temperature;
        double humidity;
        double pressure;

    public:
        WeatherStation() : temperature(25.0), humidity(60.0), pressure(1013.0) {}

        void attach(Observer *observer) override
        {
            observers.push_back(observer);
            cout << "Observer attached. Total observers: " << observers.size() << "\n";
        }

        void detach(Observer *observer) override
        {
            auto it = find(observers.begin(), observers.end(), observer);
            if (it != observers.end())
            {
                observers.erase(it);
                cout << "Observer detached. Total observers: " << observers.size() << "\n";
            }
        }

        void notify() override
        {
            cout << "Notifying " << observers.size() << " observers...\n";
            for (Observer *observer : observers)
            {
                observer->update(temperature, humidity, pressure);
            }
        }

        void setMeasurements(double temp, double hum, double press)
        {
            temperature = temp;
            humidity = hum;
            pressure = press;
            cout << "\n[WeatherStation] Measurements updated!\n";
            notify(); // Automatically notify all observers
        }
    };

    // Concrete Observers
    class CurrentConditionsDisplay : public Observer
    {
    private:
        string name;

    public:
        CurrentConditionsDisplay(const string &n) : name(n) {}

        void update(double temperature, double humidity, double pressure) override
        {
            cout << "[" << name << "] Current conditions:\n";
            cout << "  Temperature: " << temperature << "°C\n";
            cout << "  Humidity: " << humidity << "%\n";
            cout << "  Pressure: " << pressure << " hPa\n";
        }
    };

    class StatisticsDisplay : public Observer
    {
    private:
        double maxTemp;
        double minTemp;
        double sumTemp;
        int numReadings;

    public:
        StatisticsDisplay()
            : maxTemp(-100), minTemp(100), sumTemp(0), numReadings(0) {}

        void update(double temperature, double humidity, double pressure) override
        {
            sumTemp += temperature;
            numReadings++;

            if (temperature > maxTemp)
                maxTemp = temperature;
            if (temperature < minTemp)
                minTemp = temperature;

            cout << "[Statistics Display]\n";
            cout << "  Avg: " << (sumTemp / numReadings) << "°C\n";
            cout << "  Max: " << maxTemp << "°C\n";
            cout << "  Min: " << minTemp << "°C\n";
        }
    };

    class ForecastDisplay : public Observer
    {
    private:
        double lastPressure;

    public:
        ForecastDisplay() : lastPressure(1013.0) {}

        void update(double temperature, double humidity, double pressure) override
        {
            cout << "[Forecast Display] ";
            if (pressure > lastPressure)
            {
                cout << "Improving weather on the way!\n";
            }
            else if (pressure < lastPressure)
            {
                cout << "Watch out for cooler, rainy weather\n";
            }
            else
            {
                cout << "More of the same\n";
            }
            lastPressure = pressure;
        }
    };

    void demonstrate()
    {
        cout << "\n=== SOLUTION: Observer Pattern ===\n";

        WeatherStation station;

        CurrentConditionsDisplay current("Main Display");
        StatisticsDisplay stats;
        ForecastDisplay forecast;

        // Observers register themselves
        station.attach(&current);
        station.attach(&stats);
        station.attach(&forecast);

        // Measurements change - all observers notified automatically
        cout << "\n--- First Update ---\n";
        station.setMeasurements(28.0, 65.0, 1012.0);

        cout << "\n--- Second Update ---\n";
        station.setMeasurements(30.0, 70.0, 1015.0);

        // Can dynamically remove observers
        cout << "\n--- Detaching Forecast ---\n";
        station.detach(&forecast);

        cout << "\n--- Third Update ---\n";
        station.setMeasurements(25.0, 60.0, 1010.0);

        cout << "\nBenefits:\n";
        cout << "✓ Loose coupling between subject and observers\n";
        cout << "✓ Dynamic relationships (attach/detach at runtime)\n";
        cout << "✓ Broadcast communication\n";
        cout << "✓ Follows Open/Closed Principle\n";
    }
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "OBSERVER PATTERN (Publish-Subscribe)\n";
    cout << string(70, '=') << "\n";

    problem_polling::demonstrate();
    observer_solution::demonstrate();

    return 0;
}
