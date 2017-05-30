#include <iostream>
#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/gamelift/model/DescribeGameSessionsRequest.h>
#include <aws/gamelift/model/DescribeGameSessionsResult.h>
#include <aws/gamelift/GameLiftClient.h>
#include <rltk/rltk.hpp>
#include <imgui.h>
#include <imgui-sfml.h>
#include <PacketData.hpp>
#include <data/Loader.hpp>
#include "util/Director.hpp"
#include "scenes/MenuScene.hpp"
#include "static.hpp"

using asio::ip::tcp;
bool imgui_init = false;
sf::Clock deltaClock;
Director* director = nullptr;

void tick(double delta) {
    if (!imgui_init) {
        // Initialize
        ImGui::SFML::Init(*rltk::get_window());
        imgui_init = true;
    }

    ImGui::SFML::Update(*rltk::get_window(), deltaClock.restart());

    director->tick(delta);
}

int main(int argc, char** argv) {
    auto sharedLog = spdlog::stdout_logger_mt("shared", true);
    sharedLog->set_level(spdlog::level::debug);

    auto logger = spdlog::stdout_logger_mt("main", true);
    logger->set_level(spdlog::level::debug);

    PacketData::Initialize();

    dataLoader = new data::Loader("./data");

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration clientConfiguration;
#ifdef LOCAL_GAMELIFT
    clientConfiguration.endpointOverride = "127.0.0.1:8080";
    clientConfiguration.scheme = Aws::Http::Scheme::HTTP;
#else
    clientConfiguration.region = AWS_REGION;
#endif
    Aws::Auth::AWSCredentials awsCredentials;
    glClient = new Aws::GameLift::GameLiftClient(clientConfiguration);

    rltk::init(rltk::config_advanced("assets"));

    // ImGui hooks
    rltk::optional_event_hook = [] (sf::Event e) {
        ImGui::SFML::ProcessEvent(e);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
            return false;
        }
        return true;
    };

    rltk::optional_display_hook = [] () {
        ImGui::Render();
    };

    director = new Director();
    director->push(new MenuScene(director));

    rltk::run(tick);

    Aws::ShutdownAPI(options);

    return 0;
}