/* (c) 2018 ������� ����� ����������, galushin@gmail.com

������ ���� -- ����� ���������� Grabin.

Grabin -- ��� ��������� ����������� �����������: �� ������ ������������������ �� �/��� �������� ��
�� �������� ����������� ������������ �������� GNU � ��� ����, � ����� ��� ���� ������������ ������
���������� ������������ �����������; ���� ������ 3 ��������, ���� (�� ������ ������) ����� �����
������� ������.

��� ����������� ����������� ���������������� � �������, ��� ��� ����� ��������, �� ���� ������
��������; ���� ��� ������� �������� ��������� ���� ��� ����������� ��� ������������ �����.
��������� ��. � ����������� ������������ �������� GNU.

�� ������ ���� �������� ����� ����������� ������������ �������� GNU ������ � ���� �����������
�����������. ���� ��� �� ���, ��. https://www.gnu.org/licenses/.
*/

#include <grabin/view/indices.hpp>

#include <catch2/catch.hpp>
#include "../grabin_test.hpp"

TEST_CASE("range-for with indices")
{
    using Container = std::vector<int>;
    auto property = [](Container const & xs)
    {
        using Index = Container::difference_type;

        std::vector<Index> ins_obj(xs.size());
        std::iota(ins_obj.begin(), ins_obj.end(), Index(0));

        std::vector<Index> ins;
        for(auto const & i : grabin::view::indices_of(xs))
        {
            ins.push_back(i);
        }

        CHECK(ins == ins_obj);
    };

    grabin_test::check(property);
}

TEST_CASE("counter_range")
{
    using Container = std::vector<int>;
    auto property = [](Container const & xs)
    {
        using Index = Container::difference_type;

        std::vector<Index> ins_obj(xs.size());
        std::iota(ins_obj.begin(), ins_obj.end(), Index(0));

        std::vector<Index> ins;
        for(auto const & i : grabin::view::indices_of(xs))
        {
            ins.push_back(i);
        }

        CHECK(ins == ins_obj);
    };

    grabin_test::check(property);
}
