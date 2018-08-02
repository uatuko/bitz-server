
#ifndef BITZ_EVLOOP_H
#define BITZ_EVLOOP_H

#include <string>
#include <memory>

#include <uv.h>
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

		std::unique_ptr<uv_loop_t> _loop;
		std::unique_ptr<uv_tcp_t> _server;

	};

} /* end of namespace bitz */

#endif /* !BITZ_EVLOOP_H */

