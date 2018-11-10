#pragma once

#include <gcl_cpp/test.hpp>
#include <goth2018_game_project/graphics/graphics.hpp>

namespace goth2018::test::graphics
{
	struct texture_iterator
	{
		static void proceed()
		{	// 420 x 360 => 7 x 9
			goth2018::graphics::spritesheet::texture_iterator it{ { 420, 360 }, { 60, 40 } };

			GCL_TEST__EXPECT_VALUE(it.sprite_quantity, decltype(it.sprite_quantity){ 7, 9 });

			using position_type = decltype(it.calc_position(0));
			GCL_TEST__EXPECT_VALUE(it.calc_position(0), position_type{ 0,	0});
			GCL_TEST__EXPECT_VALUE(it.calc_position(1), position_type{ 60,	0});
			GCL_TEST__EXPECT_VALUE(it.calc_position(6), position_type{ 360, 0});
			GCL_TEST__EXPECT_VALUE(it.calc_position(7), position_type{ 0,	40});
			GCL_TEST__EXPECT_VALUE(it.calc_position(8), position_type{ 60,	40});

			GCL_TEST__EXPECT_VALUE(it.calc_position(it.sprite_count), it.calc_position(0));
			GCL_TEST__EXPECT_VALUE(it.calc_position(it.sprite_count + 1), it.calc_position(1));
		}
	};
}