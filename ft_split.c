/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaisogai <kaisogai@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:45:29 by kaisogai          #+#    #+#             */
/*   Updated: 2025/08/26 13:48:42 by kaisogai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "stdlib.h"

static int	count_words(const char *str, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == c)
			in_word = 0;
		str++;
	}
	return (count);
}

static int	free_strs(char **strs, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
	return (0);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (1);
	else
		return (0);
}

static void	copy_word(int word_length, char *strs, const char *str,
		char current_quote)
{
	int	k;
	int	i;

	k = 0;
	i = 0;
	while (k < word_length)
	{
		printf("[%c,%d]", str[i], i);
		if (is_quote(str[i]) && str[i] == current_quote)
		{
			i++;
			continue ;
		}
		strs[k] = str[i];
		k++;
		i++;
	}
	strs[k] = 0;
}

static int	split_words(char **strs, const char *str, int str_length, char c)
{
	int		i;
	int		word_len;
	int		j;
	int		inside_quote;
	int		quote_count;
	char	current_quote;

	i = 0;
	j = 0;
	inside_quote = 0;
	quote_count = 0;
	current_quote = ' ';
	while (i < str_length - quote_count)
	{
		word_len = 0;
		while ((str[word_len + i] && str[word_len + i] != c) || inside_quote)
		{
			word_len++;
			if (is_quote(str[word_len + i]))
			{
				if (inside_quote && str[word_len + i] != current_quote)
					continue ;
				inside_quote = !inside_quote;
				quote_count++;
				current_quote = str[word_len + i];
			}
		}
		word_len -= quote_count;
		printf("word len is %d\n", word_len);
		if (word_len > 0)
		{
			strs[j] = malloc(sizeof(char) * (word_len + 1));
			if (strs[j] == NULL)
				return (free_strs(strs, j));
			copy_word(word_len, strs[j], &(str[i]), current_quote);
			j++;
			i += word_len;
			i += quote_count;
			quote_count = 0;
		}
		else
			i++;
	}
	strs[j] = 0;
	return (1);
}

char	**ft_split(const char *str, char c)
{
	int		strs_count;
	char	**dest;
	int		i;
	int		has_no_error;

	strs_count = 1;
	i = 0;
	while (str[i] != 0)
		i++;
	strs_count = count_words(str, c);
	dest = malloc(sizeof(char *) * (strs_count + 1));
	if (dest == NULL)
		return (NULL);
	has_no_error = split_words(dest, str, i, c);
	if (!has_no_error)
		return (NULL);
	return (dest);
}

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

// int	main(void)
// {
// 	const char	*hoge = "a'b\"c' d\"''\"ef";
// 	char		**res;

// 	res = ft_split(hoge, ' ');
// 	printf("%s\n", res[0]);
// 	printf("%s\n", res[1]);
// 	free_split(res);
// }
