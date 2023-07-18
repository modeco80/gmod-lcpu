#include <lucore/Logger.hpp>

namespace lcpu {

	/// A lucore logger sink which funnels to the Source engine's
	/// Tier0 library's logging functionality.
	struct SourceSink : public lucore::Logger::Sink {
		static SourceSink& The();

		static void Init();

		void OutputMessage(const lucore::Logger::MessageData& data) override;
	};

}
