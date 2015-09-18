/* sample_plugin.cc

   Jeremy Barnes, 11 September 2015
   Copyright (c) 2015 Datacratic Inc.  All rights reserved.
 */

#include "mldb/soa/types/value_description.h"
#include "mldb/server/function.h"

using namespace Datacratic;
using namespace Datacratic::MLDB;
using namespace std;

/// We are defining a new Function here, so we need to derive from
/// Datacratic::MLDB::Function and implement that interface.
struct HelloWorldFunction: public MLDB::Function {

    /// Our constructor knows about the MLDB server we're integrated
    /// into, its configuration and includes a progress object that
    /// we can optionally use to give feedback into initialization
    /// progress (we don't use it, as initialization is intstantaneous).
    HelloWorldFunction(MLDB::MldbServer * server,
                       PolyConfig config,
                       std::function<bool (Json::Value)> onProgress)
        : MLDB::Function(server)
    {
    }

    /// Return the status of the plugin.  We return a simple constant
    /// string.  This is what will be returned from the
    /// GET /v1/functions/.../status API route.
    virtual Any getStatus() const
    {
        return std::string("A-OK");
    }

    /// Return the function information.  This tells MLDB about what the
    /// types of the input and output are, in order to allow for the
    /// function to be compiled as part of an SQL expression.  We
    /// return a single pin called "hello" with a string value, so that's
    /// what we return.
    virtual FunctionInfo getFunctionInfo() const
    {
        FunctionInfo result;
        result.output.addValue("hello", std::make_shared<MLDB::StringValueInfo>());
        return result;
    }

    /// Apply the function.  The applier contains information about the
    /// function itself, and the context gives us the input arguments.
    /// Since we don't do anything complicated or read the input, neither
    /// argument is used.
    virtual FunctionOutput apply(const FunctionApplier & applier,
                                 const FunctionContext & context) const
    {
        // Holds our result.
        FunctionOutput result;

        // Create the string "world" as of the current timestamp.  In MLDB,
        // every data point has an associated timestamp at which it's known.
        ExpressionValue world("world", Date::now());

        // Set the value of the "hello" column to our string
        result.set("hello", world);


        return result;
    }
};

// Put this declaration in "file scope" so it doesn't clash with other
// plugin initializations.
namespace {

/// Register our function with MLDB.  We say here that our function
/// hello.world takes an integer for configuration (which is ignored),
/// has the given description and has no documentation available.

static RegisterFunctionType<HelloWorldFunction, int>
regHelloWorldFunction("hello.world",
                    "Sample function that always returns hello = \"world\"",
                    "No documentation available");


} // file scope
