// SPDX-License-Identifier: GPL-2.0
// Supported chart axes
#ifndef STATS_AXIS_H
#define STATS_AXIS_H

#include <vector>
#include <QBarCategoryAxis>
#include <QCategoryAxis>
#include <QValueAxis>

namespace QtCharts {
	class QChart;
}

class StatsAxis {
public:
	virtual ~StatsAxis();
	virtual void updateLabels() = 0;
	virtual QtCharts::QAbstractAxis *qaxis() = 0;
	// Returns minimum and maximum of shown range, not of data points.
	virtual std::pair<double, double> minMax() const;
protected:
	QtCharts::QChart *chart;
	StatsAxis(QtCharts::QChart *chart, bool horizontal);
	int guessNumTicks(const QtCharts::QAbstractAxis *axis, const std::vector<QString> &strings) const;
	bool horizontal;
};

// Small template that derives from a QChart-axis and defines
// the corresponding virtual axis() accessor.
template<typename QAxis>
class StatsAxisTemplate : public StatsAxis, public QAxis
{
	using StatsAxis::StatsAxis;
	QtCharts::QAbstractAxis *qaxis() override final {
		return this;
	}
};

class ValueAxis : public StatsAxisTemplate<QtCharts::QValueAxis> {
public:
	ValueAxis(QtCharts::QChart *chart, double min, double max, int decimals, bool horizontal);
private:
	double min, max;
	int decimals;
	void updateLabels() override;
	std::pair<double, double> minMax() const override;
};

class CountAxis : public ValueAxis {
public:
	CountAxis(QtCharts::QChart *chart, int count, bool horizontal);
private:
	int count;
	void updateLabels() override;
};

class CategoryAxis : public StatsAxisTemplate<QtCharts::QBarCategoryAxis> {
public:
	CategoryAxis(QtCharts::QChart *chart, const std::vector<QString> &labels, bool horizontal);
private:
	void updateLabels();
};

struct HistogramAxisEntry {
	QString name;
	double value;
	bool recommended;
};

class HistogramAxis : public StatsAxisTemplate<QtCharts::QCategoryAxis> {
public:
	HistogramAxis(QtCharts::QChart *chart, std::vector<HistogramAxisEntry> bin_values, bool horizontal);
private:
	void updateLabels() override;
	std::pair<double, double> minMax() const override;
	std::vector<HistogramAxisEntry> bin_values;
	int preferred_step;
};

class DateAxis : public HistogramAxis {
public:
	DateAxis(QtCharts::QChart *chart, double from, double to, bool horizontal);
};

#endif