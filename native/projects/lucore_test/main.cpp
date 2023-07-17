#include <format>
#include <lucore/Assert.hpp>
#include <lucore/Logger.hpp>

int main() {
	lucore::LoggerAttachStdout();
	auto& logger = lucore::Logger::The();

	logger.Info("Hello World {}", 123.456);
	logger.Warning("sample warning");
	logger.Error("Smaple Error");

	//LUCORE_ASSERT(false, "expr should assert");
	LUCORE_CHECK(false, "should appear");
}
