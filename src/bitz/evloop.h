
#ifndef BITZ_EVLOOP_H
#define BITZ_EVLOOP_H

#include <string>
#include <memory>

#include <uv.h>
#include <spdlog/spdlog.h>
#include <icap/request.h>
#include <icap/response.h>


namespace bitz {

	class EvLoop {
	public:

		struct icap_data_t {
			std::shared_ptr<icap::Request> request;
			std::shared_ptr<icap::Response> response;
		};


		EvLoop();
		virtual ~EvLoop();

		uv_loop_t* loop();

		void run();
		void start( unsigned short port, const std::string &address = "0.0.0.0", int backlog = SOMAXCONN );
		void stop();

	private:

		static void alloc_cb( uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf );
		static void connection_cb( uv_stream_t* server, int status );
		static void read_cb( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf );
		static void shutdown_cb( uv_shutdown_t* req, int status );


		std::unique_ptr<uv_loop_t> _loop;
		std::unique_ptr<uv_tcp_t> _server;

		std::shared_ptr<spdlog::logger> _logger;

	};

} /* end of namespace bitz */

#endif /* !BITZ_EVLOOP_H */

