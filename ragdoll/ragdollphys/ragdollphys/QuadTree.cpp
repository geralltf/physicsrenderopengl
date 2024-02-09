#include "QuadTree.h"

QuadTree::QuadTree(AABB* _boundary)
{
	boundary = _boundary;
	points = new std::vector<Vector2f*>(QT_NODE_CAPACITY);
	this->shader_program = nullptr;
}
void QuadTree::subdivide()
{
	// Create four children that fully divide this quad into four quads of equal area.


}

// Insert a point into the QuadTree
bool QuadTree::insert(Vector2f* p)
{
	// Ignore objects that do not belong in this quad tree
	if (!boundary->contains_point(p)) {
		return false; // object cannot be added
	}
	// If there is space in this quad tree and if doesn't have subdivisions, add the object here
	if (points->size() < QT_NODE_CAPACITY && north_west == nullptr) {
		points->push_back(p);
		return true;
	}

	// Otherwise, subdivide and then add the point to whichever node will accept it
	if (north_west == nullptr) {
		subdivide();
	}
	// We have to add the points/data contained in this quad array to the new quads if we only want
	// the last node to hold the data

	if (north_west->insert(p)) return true;
	if (north_east->insert(p)) return true;
	if (south_west->insert(p)) return true;
	if (south_east->insert(p)) return true;

	// Otherwise, the point cannot be inserted for some unknown reason (this should never happen)
	return false;
}

// Find all points that appear within a range
std::vector<QueryRenderSample*>* QuadTree::query_render_range(AABB* range, Matrix4* camera, Vector3f* camera_pos, Quaternion* camera_orientation, float camera_angle_pitch, float camera_angle_yaw, float camera_angle_roll, Vector3f** camera_front, Vector3f** camera_up, bool show_wireframe, Vector3f* light_position)
{
	// Prepare an array of results
	std::vector<QueryRenderSample*>* samples_in_range = new std::vector<QueryRenderSample*>();

	// Automatically abort if the range does not intersect this quad
	if (!boundary->intersects_AABB(range))
	{
		std::cout << "Nothing to render." << std::endl;
		return samples_in_range; // empty list
	}



	// Check objects at this quad level
	for (int p = 0; p < points->size(); p++)
	{
		auto point = points->at(p);
		if (range->contains_point(point))
		{
			//range->render(camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up, show_wireframe);

			QueryRenderSample* sample = new QueryRenderSample();
			if (sample->sample_points == nullptr) {
				sample->sample_points = new std::vector<Vector2f*>();
			}
			sample->sample_points->push_back(point);

			if (has_init == false) 
			{
				sample->shader_program = new ShaderProgram(new std::string("scene.vs"), new std::string("scene.fs"));
				//sample->shader_program->LoadShaderProgram(new std::string("scene.vs"), SHADER_TYPE::VertexShader);
				//sample->shader_program->LoadShaderProgram(new std::string("scene.fs"), SHADER_TYPE::FragmentShader);

				sample->shader_program->init();
				sample->shader_program->has_init = true;

				if (render_samples == nullptr) {
					render_samples = new std::vector<QueryRenderSample*>();
				}
				render_samples->push_back(sample);

				samples_in_range->push_back(sample);

				has_init = true;
			}
			else 
			{
				samples_in_range = render_samples;
			}
		}
	}

	// Terminate here, if there are no children
	if (north_west == nullptr && north_east == nullptr && south_west == nullptr && south_east == nullptr)
	{
		return samples_in_range;
	}

	// Otherwise, add the points from the children
	if (north_west != nullptr)
	{
		auto list = north_west->query_render_range(range, camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up, show_wireframe, light_position);
		auto start = list->begin();
		auto end = list->end();
		samples_in_range->insert(samples_in_range->end(), start, end);
	}

	if (north_east != nullptr)
	{
		auto list = north_east->query_render_range(range, camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up, show_wireframe, light_position);
		auto start = list->begin();
		auto end = list->end();
		samples_in_range->insert(samples_in_range->end(), start, end);
	}

	if (south_west != nullptr)
	{
		auto list = south_west->query_render_range(range, camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up, show_wireframe, light_position);
		auto start = list->begin();
		auto end = list->end();
		samples_in_range->insert(samples_in_range->end(), start, end);
	}

	if (south_east != nullptr)
	{
		auto list = south_east->query_render_range(range, camera, camera_pos, camera_orientation, camera_angle_pitch, camera_angle_yaw, camera_angle_roll, camera_front, camera_up, show_wireframe, light_position);
		auto start = list->begin();
		auto end = list->end();
		samples_in_range->insert(samples_in_range->end(), start, end);
	}

	return samples_in_range;
}

// Find all points that appear within a range
std::vector<Vector2f*>* QuadTree::query_range(AABB* range)
{
	// Prepare an array of results
	std::vector<Vector2f*>* points_in_range = new std::vector<Vector2f*>();

	// Automatically abort if the range does not intersect this quad
	if (!boundary->intersects_AABB(range))
	{
		return points_in_range; // empty list
	}

	// Check objects at this quad level
	for (int p = 0; p < points->size(); p++)
	{
		if (range->contains_point(points->at(p)))
		{
			points_in_range->push_back(points->at(p));
		}
	}

	// Terminate here, if there are no children
	if (north_west == nullptr && north_east == nullptr && south_west == nullptr && south_east == nullptr)
	{
		return points_in_range;
	}

	// Otherwise, add the points from the children
	if (north_west != nullptr)
	{
		auto list = north_west->query_range(range);
		auto start = list->begin();
		auto end = list->end();
		points_in_range->insert(points_in_range->end(), start, end);
	}

	if (north_east != nullptr)
	{
		auto list = north_east->query_range(range);
		auto start = list->begin();
		auto end = list->end();
		points_in_range->insert(points_in_range->end(), start, end);
	}

	if (south_west != nullptr)
	{
		auto list = south_west->query_range(range);
		auto start = list->begin();
		auto end = list->end();
		points_in_range->insert(points_in_range->end(), start, end);
	}

	if (south_east != nullptr)
	{
		auto list = south_east->query_range(range);
		auto start = list->begin();
		auto end = list->end();
		points_in_range->insert(points_in_range->end(), start, end);
	}

	return points_in_range;
}