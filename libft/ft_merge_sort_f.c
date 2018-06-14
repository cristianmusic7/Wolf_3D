/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_merge_sort_f.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cfranco <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 23:16:00 by cfranco           #+#    #+#             */
/*   Updated: 2018/06/12 23:16:42 by cfranco          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	merge(double *nums, int start, int mid, int end)
{
	int i_copy;
	int i_left;
	int i_right;
	double copy[end - start + 1];

	i_copy = 0;
	i_left = start;
	i_right = mid + 1;
	while (i_left <= mid && i_right <= end)
	{
		if (nums[i_left] < nums[i_right])
			copy[i_copy++] = nums[i_left++];
		else
			copy[i_copy++] = nums[i_right++];
	}
	while (i_left <= mid)
		copy[i_copy++] = nums[i_left++];
	while (i_right <= end)
		copy[i_copy++] = nums[i_right++];
	i_copy = 0;
	while (i_copy < (end - start + 1))
	{
		nums[start + i_copy] = copy[i_copy];
		i_copy++;
	}
}

static void	merge_sort_recursion(double *nums, int start, int end)
{
	int mid;

	if (start == end)
		return ;
	mid = (start + end) / 2;
	merge_sort_recursion(nums, start, mid);
	merge_sort_recursion(nums, mid + 1, end);
	merge(nums, start, mid, end);
}

/*
** ft_sort_merge_f() takes a list of floats and the length of the array and
** then sorts the numbers recursively in nlog(n) time
*/

void		ft_sort_merge_f(double *nums, int len)
{
	merge_sort_recursion(nums, 0, len - 1);
}
