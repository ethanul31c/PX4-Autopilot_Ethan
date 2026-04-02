/****************************************************************************
 *
 *   Copyright (c) 2021-2022 PX4 Development Team. All rights reserved.
 *
*	Added on March 25th, 2026 by YiSin Tsang
 ****************************************************************************/

#ifndef CUSTOM_MARCH_HPP	// 1
#define CUSTOM_MARCH_HPP	// 2

#include <uORB/topics/custom_march.h>	// 3

class MavlinkStreamCustomMarch : public MavlinkStream // 4
{
public:
	static MavlinkStream *new_instance(Mavlink *mavlink) { return new MavlinkStreamCustomMarch(mavlink); } // 5

	static constexpr const char *get_name_static() { return "CUSTOM_MARCH"; }	// 6
	static constexpr uint16_t get_id_static() { return MAVLINK_MSG_ID_CUSTOM_MARCH; }	// 7

	const char *get_name() const override { return get_name_static(); }
	uint16_t get_id() override { return get_id_static(); }

	unsigned get_size() override
	{
		return _custom_march_sub.advertised() ? MAVLINK_MSG_ID_CUSTOM_MARCH_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES : 0;
	}

private:
	explicit MavlinkStreamCustomMarch(Mavlink *mavlink) : MavlinkStream(mavlink) {}

	uORB::Subscription _custom_march_sub{ORB_ID(custom_march)};

	bool send() override
	{
		custom_march_s custom_march_data;

		if (_custom_march_sub.update(&custom_march_data)) {
			mavlink_custom_march_t msg{};
			msg.custom_value= custom_march_data.custom_value;
			msg.value 	= custom_march_data.value;

			mavlink_msg_custom_march_send_struct(_mavlink->get_channel(), &msg);
			return true;
		}

		return false;
	}
};

#endif // CUSTOM_MARCH_HPP
