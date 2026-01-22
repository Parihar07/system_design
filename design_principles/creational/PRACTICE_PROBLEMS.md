# Creational Design Patterns - Practice Problems

Solve these problems and implement them in separate files. I'll review your solutions.

---

## Problem 1: Logger System (Singleton) - EASY

**Scenario:** You're building a multi-threaded application that needs centralized logging.

**Requirements:**
1. Only one Logger instance should exist
2. Log messages should be stored in memory
3. Support different log levels: INFO, WARNING, ERROR
4. Thread-safe log writing
5. Ability to query total logs per level
6. Save all logs to file on demand

**API Example:**
```cpp
Logger& log = Logger::getInstance();
log.info("Application started");
log.error("Connection failed");
log.getStats();  // Shows count per level
log.saveToFile("app.log");
```

**File:** `problem1_logger.cpp`

**Evaluation Criteria:**
- Private constructor
- Deleted copy constructor/assignment
- Meyer's Singleton or thread-safe approach
- Clean API design

---

## Problem 2: Document Creator (Factory Method) - EASY

**Scenario:** Build a document creation system for different file formats.

**Requirements:**
1. Support PDF, Word, and Text documents
2. Each document type has different:
   - Open behavior
   - Save behavior  
   - Export behavior
3. Application should be extensible (easy to add new formats)
4. Client code shouldn't depend on concrete document classes

**API Example:**
```cpp
unique_ptr<Application> app = make_unique<PDFApplication>();
app->createNewDocument();  // Creates PDF document
app->openDocument();
app->saveDocument();
```

**File:** `problem2_document.cpp`

**Evaluation Criteria:**
- Abstract Product and Creator
- Concrete implementations for each format
- No if-else ladder in client code
- Open/Closed Principle compliance

---

## Problem 3: Database Connection Pool (Singleton + Factory) - MEDIUM

**Scenario:** Create a connection pool manager that handles multiple database types.

**Requirements:**
1. Single ConnectionPoolManager instance (Singleton)
2. Support MySQL, PostgreSQL, MongoDB connections
3. Pool should maintain max 5 connections per database type
4. Reuse idle connections
5. Create new connection if pool not full
6. Factory method to create appropriate connection type

**API Example:**
```cpp
ConnectionPoolManager& pool = ConnectionPoolManager::getInstance();
Connection* conn = pool.getConnection("MySQL");
conn->executeQuery("SELECT * FROM users");
pool.releaseConnection(conn);  // Return to pool
```

**File:** `problem3_connection_pool.cpp`

**Evaluation Criteria:**
- Singleton for pool manager
- Factory for connection creation
- Resource management (pool logic)
- Proper memory management

---

## Problem 4: Notification System (Abstract Factory) - MEDIUM

**Scenario:** Build a cross-platform notification system (Mobile, Desktop, Web).

**Requirements:**
1. Each platform needs:
   - Alert notification
   - Email notification
   - SMS notification
2. Platform-specific styling and behavior
3. Factory creates complete family of notifications for a platform
4. Easy to add new platforms

**API Example:**
```cpp
unique_ptr<NotificationFactory> factory = make_unique<MobileFactory>();
auto alert = factory->createAlert();
auto email = factory->createEmail();
alert->send("System update available");
```

**File:** `problem4_notifications.cpp`

**Evaluation Criteria:**
- Abstract Factory interface
- Concrete factories per platform
- Product families properly created
- No mixing of platform components

---

## Problem 5: HTTP Request Builder (Builder Pattern) - MEDIUM

**Scenario:** Create a fluent HTTP request builder for REST API calls.

**Requirements:**
1. Support building complex HTTP requests step-by-step
2. Optional components: headers, query params, body, auth
3. Different request types: GET, POST, PUT, DELETE
4. Validation before sending request
5. Fluent interface (method chaining)

**API Example:**
```cpp
HttpRequest request = HttpRequestBuilder()
    .setUrl("https://api.example.com/users")
    .setMethod(HttpMethod::POST)
    .addHeader("Content-Type", "application/json")
    .setBody("{\"name\":\"John\"}")
    .setTimeout(5000)
    .build();

request.send();
```

**File:** `problem5_http_builder.cpp`

**Evaluation Criteria:**
- Fluent interface implementation
- Clear separation: Builder vs Product
- Validation logic
- Method chaining returns `*this`

---

## Problem 6: Game Character Cloning (Prototype) - MEDIUM

**Scenario:** Game engine needs to clone characters with all their equipment and stats.

**Requirements:**
1. Support deep copying of complex character objects
2. Characters have: weapons, armor, inventory, buffs
3. Clone method creates exact replica
4. Registry of prototype characters (templates)
5. Ability to clone and customize

**API Example:**
```cpp
CharacterRegistry& registry = CharacterRegistry::getInstance();
registry.registerPrototype("Warrior", warriorPrototype);

Character* newWarrior = registry.clone("Warrior");
newWarrior->setName("Aragon");
newWarrior->addWeapon(legendarySword);
```

**File:** `problem6_character_clone.cpp`

**Evaluation Criteria:**
- Clone method implementation
- Deep copy vs shallow copy handling
- Prototype registry pattern
- Copy constructor usage

---

## Problem 7: Plugin Architecture (Factory + Singleton) - HARD

**Scenario:** Build an extensible plugin system for a text editor.

**Requirements:**
1. PluginManager (Singleton) manages all plugins
2. Support different plugin types: Formatter, Linter, Syntax Highlighter
3. Plugins can be loaded dynamically (simulated with factory)
4. Each plugin type has factory
5. Register plugins at runtime
6. Query available plugins by type

**API Example:**
```cpp
PluginManager& pm = PluginManager::getInstance();
pm.registerPlugin("prettier", new FormatterPlugin());
pm.registerPlugin("eslint", new LinterPlugin());

Plugin* formatter = pm.getPlugin("prettier");
formatter->execute(code);
```

**File:** `problem7_plugin_system.cpp`

**Evaluation Criteria:**
- Singleton manager
- Factory for plugin creation
- Plugin interface design
- Dynamic registration/lookup

---

## Problem 8: Multi-Format Config Loader (Abstract Factory + Builder) - HARD

**Scenario:** Configuration system supporting JSON, YAML, XML with validation.

**Requirements:**
1. Abstract factory creates: Parser, Validator, Serializer for each format
2. Builder constructs complex Config object from parsed data
3. Support nested configurations
4. Validation rules per format
5. Easy to add new formats (e.g., TOML)

**API Example:**
```cpp
ConfigFactory* factory = getFactory("json");
Parser* parser = factory->createParser();
Config config = parser->parse("config.json")
    .validate()
    .build();

cout << config.get("database.host");
```

**File:** `problem8_config_loader.cpp`

**Evaluation Criteria:**
- Abstract Factory for format families
- Builder for Config construction
- Proper pattern combination
- Clean separation of concerns

---

## How to Submit

1. Create your solution file (e.g., `problem1_logger.cpp`)
2. Test it compiles and runs: `make FILE=problem1_logger.cpp run`
3. Ask me to review when ready

## Evaluation Rubric

- **Pattern Choice** (30%): Correct pattern selection and justification
- **Implementation** (30%): Code quality, SOLID principles
- **Design** (20%): Clean API, extensibility
- **Thread Safety** (10%): When applicable
- **Memory Management** (10%): RAII, smart pointers

Start with Problem 1 and work your way up. Ask questions if requirements unclear!
