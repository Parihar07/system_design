/**
 * BUILDER PATTERN
 *
 * Intent: Separate the construction of a complex object from its representation,
 *         allowing the same construction process to create different representations.
 *
 * Key Characteristics:
 * - Step-by-step construction
 * - Fluent interface (method chaining)
 * - Optional parameters
 * - Validation before object creation
 * - Immutable product (usually)
 *
 * When to Use:
 * - Object has many optional parameters
 * - Construction requires multiple steps
 * - Want to avoid telescoping constructors
 * - Need different representations of same construction process
 * - Want readable, self-documenting code
 *
 * Common Use Cases:
 * - HTTP request builders
 * - SQL query builders
 * - Document builders (HTML, XML)
 * - Configuration objects
 * - Test data builders
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

using namespace std;

// ============================================================================
// PROBLEM: Telescoping Constructors
// ============================================================================

namespace problem_telescoping
{
    class HttpRequest
    {
    private:
        string url;
        string method;
        map<string, string> headers;
        string body;
        int timeout;
        bool followRedirects;
        int maxRetries;

    public:
        // Problem: Too many constructors!
        HttpRequest(const string &url)
            : url(url), method("GET"), timeout(3000), followRedirects(true), maxRetries(0) {}

        HttpRequest(const string &url, const string &method)
            : url(url), method(method), timeout(3000), followRedirects(true), maxRetries(0) {}

        HttpRequest(const string &url, const string &method, int timeout)
            : url(url), method(method), timeout(timeout), followRedirects(true), maxRetries(0) {}

        HttpRequest(const string &url, const string &method, int timeout, bool redirects)
            : url(url), method(method), timeout(timeout), followRedirects(redirects), maxRetries(0) {}

        HttpRequest(const string &url, const string &method, int timeout, bool redirects, int retries)
            : url(url), method(method), timeout(timeout), followRedirects(redirects), maxRetries(retries) {}

        // What if we need to add more parameters?
        // This becomes unmanageable!

        void display()
        {
            cout << "Request: " << method << " " << url << "\n";
            cout << "Timeout: " << timeout << "ms\n";
        }
    };

    void demonstrate()
    {
        cout << "=== PROBLEM: Telescoping Constructors ===\n";

        // Confusing: Which parameter is which?
        HttpRequest req1("https://api.example.com", "POST", 5000, true, 3);
        req1.display();

        cout << "\nProblems:\n";
        cout << "❌ Hard to remember parameter order\n";
        cout << "❌ Can't skip parameters in the middle\n";
        cout << "❌ Must provide all parameters or use default constructor\n";
        cout << "❌ Adding new parameters requires new constructors\n";
        cout << "❌ Not self-documenting (what does 'true' mean?)\n";
    }
}

// ============================================================================
// SOLUTION: Builder Pattern
// ============================================================================

namespace builder_solution
{
    // Product
    class HttpRequest
    {
    private:
        string url;
        string method;
        map<string, string> headers;
        map<string, string> queryParams;
        string body;
        int timeout;
        bool followRedirects;
        int maxRetries;

        // Private constructor - can only be built via Builder
        HttpRequest() : method("GET"), timeout(3000), followRedirects(true), maxRetries(0) {}

        friend class HttpRequestBuilder;

    public:
        void display() const
        {
            cout << "\n=== HTTP Request ===\n";
            cout << "Method: " << method << "\n";
            cout << "URL: " << url;

            if (!queryParams.empty())
            {
                cout << "?";
                bool first = true;
                for (const auto &[key, value] : queryParams)
                {
                    if (!first)
                        cout << "&";
                    cout << key << "=" << value;
                    first = false;
                }
            }
            cout << "\n";

            if (!headers.empty())
            {
                cout << "Headers:\n";
                for (const auto &[key, value] : headers)
                {
                    cout << "  " << key << ": " << value << "\n";
                }
            }

            if (!body.empty())
            {
                cout << "Body: " << body << "\n";
            }

            cout << "Timeout: " << timeout << "ms\n";
            cout << "Follow Redirects: " << (followRedirects ? "Yes" : "No") << "\n";
            cout << "Max Retries: " << maxRetries << "\n";
        }

        string getUrl() const { return url; }
        string getMethod() const { return method; }
    };

    // Builder
    class HttpRequestBuilder
    {
    private:
        HttpRequest request;

    public:
        HttpRequestBuilder &setUrl(const string &url)
        {
            request.url = url;
            return *this; // Enable method chaining
        }

        HttpRequestBuilder &setMethod(const string &method)
        {
            request.method = method;
            return *this;
        }

        HttpRequestBuilder &addHeader(const string &key, const string &value)
        {
            request.headers[key] = value;
            return *this;
        }

        HttpRequestBuilder &addQueryParam(const string &key, const string &value)
        {
            request.queryParams[key] = value;
            return *this;
        }

        HttpRequestBuilder &setBody(const string &body)
        {
            request.body = body;
            return *this;
        }

        HttpRequestBuilder &setTimeout(int timeout)
        {
            request.timeout = timeout;
            return *this;
        }

        HttpRequestBuilder &setFollowRedirects(bool follow)
        {
            request.followRedirects = follow;
            return *this;
        }

        HttpRequestBuilder &setMaxRetries(int retries)
        {
            request.maxRetries = retries;
            return *this;
        }

        // Validation and building
        HttpRequest build()
        {
            if (request.url.empty())
            {
                throw runtime_error("URL is required!");
            }

            if (request.method != "GET" && request.method != "POST" &&
                request.method != "PUT" && request.method != "DELETE")
            {
                throw runtime_error("Invalid HTTP method!");
            }

            if (request.timeout < 0)
            {
                throw runtime_error("Timeout cannot be negative!");
            }

            return request;
        }
    };

    void demonstrate()
    {
        cout << "\n=== SOLUTION: Builder Pattern ===\n";

        // Example 1: Simple GET request
        HttpRequest req1 = HttpRequestBuilder()
                               .setUrl("https://api.example.com/users")
                               .setMethod("GET")
                               .addHeader("Authorization", "Bearer token123")
                               .setTimeout(5000)
                               .build();

        req1.display();

        // Example 2: POST request with body
        HttpRequest req2 = HttpRequestBuilder()
                               .setUrl("https://api.example.com/users")
                               .setMethod("POST")
                               .addHeader("Content-Type", "application/json")
                               .setBody("{\"name\":\"John\",\"age\":30}")
                               .setTimeout(10000)
                               .setMaxRetries(3)
                               .build();

        req2.display();

        // Example 3: GET with query parameters
        HttpRequest req3 = HttpRequestBuilder()
                               .setUrl("https://api.example.com/search")
                               .setMethod("GET")
                               .addQueryParam("q", "builder pattern")
                               .addQueryParam("page", "1")
                               .addQueryParam("limit", "10")
                               .build();

        req3.display();

        cout << "\nBenefits:\n";
        cout << "✓ Readable, self-documenting code\n";
        cout << "✓ Optional parameters handled cleanly\n";
        cout << "✓ Can set parameters in any order\n";
        cout << "✓ Validation before object creation\n";
        cout << "✓ Fluent interface (method chaining)\n";
    }
}

// ============================================================================
// REAL-WORLD EXAMPLE: Pizza Builder
// ============================================================================

namespace pizza_builder
{
    enum class Size
    {
        SMALL,
        MEDIUM,
        LARGE
    };
    enum class Crust
    {
        THIN,
        THICK,
        STUFFED
    };

    class Pizza
    {
    private:
        Size size;
        Crust crust;
        bool cheese;
        vector<string> toppings;
        bool extraSauce;
        string specialInstructions;

        Pizza() : size(Size::MEDIUM), crust(Crust::THIN), cheese(true), extraSauce(false) {}

        friend class PizzaBuilder;

    public:
        void display() const
        {
            cout << "\n=== Pizza Order ===\n";
            cout << "Size: ";
            switch (size)
            {
            case Size::SMALL:
                cout << "Small";
                break;
            case Size::MEDIUM:
                cout << "Medium";
                break;
            case Size::LARGE:
                cout << "Large";
                break;
            }
            cout << "\n";

            cout << "Crust: ";
            switch (crust)
            {
            case Crust::THIN:
                cout << "Thin";
                break;
            case Crust::THICK:
                cout << "Thick";
                break;
            case Crust::STUFFED:
                cout << "Stuffed";
                break;
            }
            cout << "\n";

            cout << "Cheese: " << (cheese ? "Yes" : "No") << "\n";
            cout << "Extra Sauce: " << (extraSauce ? "Yes" : "No") << "\n";

            if (!toppings.empty())
            {
                cout << "Toppings: ";
                for (size_t i = 0; i < toppings.size(); ++i)
                {
                    cout << toppings[i];
                    if (i < toppings.size() - 1)
                        cout << ", ";
                }
                cout << "\n";
            }

            if (!specialInstructions.empty())
            {
                cout << "Special Instructions: " << specialInstructions << "\n";
            }
        }

        double calculatePrice() const
        {
            double price = 0.0;
            switch (size)
            {
            case Size::SMALL:
                price = 8.99;
                break;
            case Size::MEDIUM:
                price = 12.99;
                break;
            case Size::LARGE:
                price = 16.99;
                break;
            }
            price += toppings.size() * 1.50;
            if (extraSauce)
                price += 0.99;
            if (crust == Crust::STUFFED)
                price += 2.99;
            return price;
        }
    };

    class PizzaBuilder
    {
    private:
        Pizza pizza;

    public:
        PizzaBuilder &setSize(Size size)
        {
            pizza.size = size;
            return *this;
        }

        PizzaBuilder &setCrust(Crust crust)
        {
            pizza.crust = crust;
            return *this;
        }

        PizzaBuilder &withCheese(bool cheese = true)
        {
            pizza.cheese = cheese;
            return *this;
        }

        PizzaBuilder &addTopping(const string &topping)
        {
            pizza.toppings.push_back(topping);
            return *this;
        }

        PizzaBuilder &withExtraSauce(bool extra = true)
        {
            pizza.extraSauce = extra;
            return *this;
        }

        PizzaBuilder &setSpecialInstructions(const string &instructions)
        {
            pizza.specialInstructions = instructions;
            return *this;
        }

        Pizza build()
        {
            return pizza;
        }
    };

    void demonstrate()
    {
        cout << "\n=== PIZZA BUILDER EXAMPLE ===\n";

        // Simple pizza
        Pizza margherita = PizzaBuilder()
                               .setSize(Size::MEDIUM)
                               .setCrust(Crust::THIN)
                               .withCheese()
                               .build();

        margherita.display();
        cout << "Price: $" << margherita.calculatePrice() << "\n";

        // Custom pizza
        Pizza supreme = PizzaBuilder()
                            .setSize(Size::LARGE)
                            .setCrust(Crust::STUFFED)
                            .withCheese()
                            .addTopping("Pepperoni")
                            .addTopping("Mushrooms")
                            .addTopping("Olives")
                            .addTopping("Bell Peppers")
                            .withExtraSauce()
                            .setSpecialInstructions("Well done, cut into 8 slices")
                            .build();

        supreme.display();
        cout << "Price: $" << supreme.calculatePrice() << "\n";
    }
}

// ============================================================================
// ADVANCED EXAMPLE: SQL Query Builder
// ============================================================================

namespace sql_query_builder
{
    class SqlQuery
    {
    private:
        vector<string> selectColumns;
        string fromTable;
        vector<string> whereClauses;
        vector<string> joinClauses;
        vector<string> orderByColumns;
        int limitValue;
        int offsetValue;

        SqlQuery() : limitValue(-1), offsetValue(0) {}

        friend class SqlQueryBuilder;

    public:
        string toString() const
        {
            stringstream ss;

            // SELECT
            ss << "SELECT ";
            if (selectColumns.empty())
            {
                ss << "*";
            }
            else
            {
                for (size_t i = 0; i < selectColumns.size(); ++i)
                {
                    ss << selectColumns[i];
                    if (i < selectColumns.size() - 1)
                        ss << ", ";
                }
            }

            // FROM
            ss << "\nFROM " << fromTable;

            // JOIN
            for (const auto &join : joinClauses)
            {
                ss << "\n"
                   << join;
            }

            // WHERE
            if (!whereClauses.empty())
            {
                ss << "\nWHERE ";
                for (size_t i = 0; i < whereClauses.size(); ++i)
                {
                    ss << whereClauses[i];
                    if (i < whereClauses.size() - 1)
                        ss << " AND ";
                }
            }

            // ORDER BY
            if (!orderByColumns.empty())
            {
                ss << "\nORDER BY ";
                for (size_t i = 0; i < orderByColumns.size(); ++i)
                {
                    ss << orderByColumns[i];
                    if (i < orderByColumns.size() - 1)
                        ss << ", ";
                }
            }

            // LIMIT
            if (limitValue > 0)
            {
                ss << "\nLIMIT " << limitValue;
            }

            // OFFSET
            if (offsetValue > 0)
            {
                ss << "\nOFFSET " << offsetValue;
            }

            return ss.str();
        }
    };

    class SqlQueryBuilder
    {
    private:
        SqlQuery query;

    public:
        SqlQueryBuilder &select(const vector<string> &columns)
        {
            query.selectColumns = columns;
            return *this;
        }

        SqlQueryBuilder &from(const string &table)
        {
            query.fromTable = table;
            return *this;
        }

        SqlQueryBuilder &where(const string &condition)
        {
            query.whereClauses.push_back(condition);
            return *this;
        }

        SqlQueryBuilder &join(const string &table, const string &condition)
        {
            query.joinClauses.push_back("JOIN " + table + " ON " + condition);
            return *this;
        }

        SqlQueryBuilder &leftJoin(const string &table, const string &condition)
        {
            query.joinClauses.push_back("LEFT JOIN " + table + " ON " + condition);
            return *this;
        }

        SqlQueryBuilder &orderBy(const string &column)
        {
            query.orderByColumns.push_back(column);
            return *this;
        }

        SqlQueryBuilder &limit(int limit)
        {
            query.limitValue = limit;
            return *this;
        }

        SqlQueryBuilder &offset(int offset)
        {
            query.offsetValue = offset;
            return *this;
        }

        SqlQuery build()
        {
            if (query.fromTable.empty())
            {
                throw runtime_error("FROM clause is required!");
            }
            return query;
        }
    };

    void demonstrate()
    {
        cout << "\n=== SQL QUERY BUILDER EXAMPLE ===\n";

        // Simple query
        SqlQuery query1 = SqlQueryBuilder()
                              .select({"id", "name", "email"})
                              .from("users")
                              .where("active = 1")
                              .orderBy("name")
                              .limit(10)
                              .build();

        cout << "\n--- Query 1 ---\n";
        cout << query1.toString() << "\n";

        // Complex query with joins
        SqlQuery query2 = SqlQueryBuilder()
                              .select({"u.name", "o.order_date", "o.total"})
                              .from("users u")
                              .join("orders o", "u.id = o.user_id")
                              .leftJoin("shipping s", "o.id = s.order_id")
                              .where("o.status = 'completed'")
                              .where("o.total > 100")
                              .orderBy("o.order_date DESC")
                              .limit(20)
                              .offset(10)
                              .build();

        cout << "\n--- Query 2 (Complex) ---\n";
        cout << query2.toString() << "\n";
    }
}

// ============================================================================
// COMPARISON AND GUIDELINES
// ============================================================================

void printGuidelines()
{
    cout << "\n"
         << string(70, '=') << "\n";
    cout << "BUILDER PATTERN - KEY POINTS\n";
    cout << string(70, '=') << "\n\n";

    cout << "WHEN TO USE:\n";
    cout << "  ✓ Object has 4+ constructor parameters\n";
    cout << "  ✓ Many parameters are optional\n";
    cout << "  ✓ Want immutable objects\n";
    cout << "  ✓ Need validation before creation\n";
    cout << "  ✓ Step-by-step construction required\n\n";

    cout << "BENEFITS:\n";
    cout << "  • Readable, self-documenting code\n";
    cout << "  • Fluent interface (method chaining)\n";
    cout << "  • Immutable products\n";
    cout << "  • Validation at build time\n";
    cout << "  • Avoids telescoping constructors\n\n";

    cout << "IMPLEMENTATION TIPS:\n";
    cout << "  1. Return *this for method chaining\n";
    cout << "  2. Make product constructor private\n";
    cout << "  3. Validate in build() method\n";
    cout << "  4. Use friend class for access\n";
    cout << "  5. Consider separate Director class for complex flows\n\n";

    cout << "BUILDER vs FACTORY:\n";
    cout << "  Factory: Creates object in one shot\n";
    cout << "  Builder: Creates object step-by-step\n\n";

    cout << "COMMON USES:\n";
    cout << "  • HTTP clients (requests/responses)\n";
    cout << "  • SQL query builders\n";
    cout << "  • Document builders (HTML/XML)\n";
    cout << "  • Configuration objects\n";
    cout << "  • Test data builders\n";
}

// ============================================================================
// MAIN
// ============================================================================

int main()
{
    cout << "BUILDER PATTERN\n";
    cout << string(70, '=') << "\n";

    problem_telescoping::demonstrate();
    builder_solution::demonstrate();
    pizza_builder::demonstrate();
    sql_query_builder::demonstrate();

    printGuidelines();

    cout << "\nKEY TAKEAWAY:\n";
    cout << "Builder separates construction from representation,\n";
    cout << "providing a clean, fluent API for complex object creation.\n";

    return 0;
}
