#ifndef UTIL_HTTP_RESP_HPP
#define UTIL_HTTP_RESP_HPP


#include <vector>
#include <string>
#include <cstring>

#include <utility/http_response.h>


namespace util 
{


/******************************************************************************/
typedef struct _http_response {
	uint16_t status_code;
	std::vector<std::string> header_fields;
	std::vector<std::string> header_values;
	uint8_t *body;
	size_t body_len;

	_http_response()
		: status_code(0), body(nullptr), body_len(0) 
	{}

	~_http_response()
	{
		if (body) {
			delete[] body;
			body = nullptr;
			body_len = 0;
		}
	}


/******************************************************************************/
	void fill_http_response(HttpResponse *resp)
	{
		status_code = resp->get_status_code();

		const auto &fields = resp->get_headers_fields();
		const auto &values = resp->get_headers_values();

		header_fields.clear();
		header_values.clear();

		for (size_t i = 0; i < fields.size() && i < values.size(); ++i) {
			header_fields.push_back(std::string(*fields[i]));
			header_values.push_back(std::string(*values[i]));
		}

		if (body) {
			delete[] body;
			body = nullptr;
			body_len = 0;
		}

		uint8_t *resp_body = static_cast<uint8_t *>(resp->get_body());
		size_t length = resp->get_body_length();

		body = new uint8_t[length];
		std::memcpy(body, resp_body, length);
		body_len = length;
	}


/******************************************************************************/
	std::string to_string()
	{
		std::string out;
		size_t h_fields_size = header_fields.size();
		size_t h_values_size = header_values.size();

		out += "Status code: " + std::to_string(status_code) + "\n";

		for (size_t i = 0; i < h_fields_size && i < h_values_size; ++i) {
			out += header_fields[i] + ": " + header_values[i] + "\n";
		}

		out += "Body length: " + std::to_string(body_len) + "\n";

		return out;
	}


/******************************************************************************/
	void http_response_to_image()
	{

	}


} http_response_t; 


}	// namespace util 


#endif	// #ifndef UTIL_HTTP_RESP_HPP