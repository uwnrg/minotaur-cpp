#ifndef MINOTAUR_CPP_STATUSBOX_H
#define MINOTAUR_CPP_STATUSBOX_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

#include <memory>
#include <unordered_map>

/**
 * Status label is a QLabel with an attached ID so
 * that one can be removed from the StatusBox.
 */
class StatusLabel : public QLabel {
public:
    StatusLabel(QWidget *parent, std::size_t id, const QString &initial);

    std::size_t id() const;

private:
    std::size_t m_id;
};

/**
 * A Dialog instance to which any QObject can add a label, receive
 * a reference to that label, and modify its constents. The labels
 * are managed by this class.
 */
class StatusBox : public QDialog {
Q_OBJECT

public:
    explicit StatusBox(QWidget *parent = nullptr);

    /**
     * Create a label with an initial string value.
     *
     * @param initial string label value
     * @return reference to the label
     */
    StatusLabel *add_label(const QString &initial);

    /**
     * Remove the label from the action box. Objects that manage
     * a label should call this in their destructors if the
     * labels are not removed elsewhere.
     *
     * @param label reference to label to remove
     */
    void remove_label(StatusLabel *label);

private:
    /**
     * Incrementing ID value so that labels are easily tracked.
     */
    std::size_t m_id;
    /**
     * Layout where the labels are placed.
     */
    std::unique_ptr<QVBoxLayout> m_layout;
    /**
     * Map of ID to label pointer for easy lookup.
     */
    std::unordered_map<std::size_t, std::unique_ptr<StatusLabel>> m_labels;
};

#endif //MINOTAUR_CPP_STATUSBOX_H
